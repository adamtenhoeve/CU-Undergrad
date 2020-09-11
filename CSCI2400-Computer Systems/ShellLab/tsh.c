/*
 * tsh - A tiny shell program with job control
 *
 * Adam Ten Hoeve adte5487
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>

/* TEST CASES
 * 01 - SUCCESS
 * 02 - SUCCESS
 * 03 - SUCCESS
 * 04 - SUCCESS
 * 05 - SUCCESS
 * 06 - SUCCESS
 * 07 - SUCCESS
 * 08 - SUCCESS
 * 09 - SUCCESS
 * 10 - SUCCESS
 * 11 - SUCCESS
 * 12 - SUCCESS
 * 13 - SUCCESS
 * 14 - SUCCESS
 * 15 - SUCCESS
 * 16 - SUCCESS
 */

/* Misc manifest constants */
#define MAXLINE    1024   /* max line size */
#define MAXARGS     128   /* max args on a command line */
#define MAXJOBS      16   /* max jobs at any point in time */
#define MAXJID    1<<16   /* max job ID */

/* Job states */
#define UNDEF 0 /* undefined */
#define FG 1    /* running in foreground */
#define BG 2    /* running in background */
#define ST 3    /* stopped */

/* Builtin types */
#define BLTN_UNK 0
#define BLTN_IGNR 1
#define BLTN_BGFG 2
#define BLTN_JOBS 3
#define BLTN_EXIT 4
#define BLTN_KILLALL 5

/*
 * Jobs states: FG (foreground), BG (background), ST (stopped)
 * Job state transitions and enabling actions:
 *     FG -> ST  : ctrl-z
 *     ST -> FG  : fg command
 *     ST -> BG  : bg command
 *     BG -> FG  : fg command
 * At most 1 job can be in the FG state.
 */

/* Global variables */
extern char **environ;      /* defined in libc */
char prompt[] = "tsh> ";    /* command line prompt (DO NOT CHANGE) */
int verbose = 0;            /* if true, print additional output */
int nextjid = 1;            /* next job ID to allocate */
char sbuf[MAXLINE];         /* for composing sprintf messages */

// [2] (6270) Foreground /bin/ps
// [JOB ID] (PID) STATE /COMMAND

struct job_t {              /* The job struct */
    pid_t pid;              /* job PID */
    int jid;                /* job ID [1, 2, ...] */
    int state;              /* UNDEF, BG, FG, or ST */
    char cmdline[MAXLINE];  /* command line */
};
struct job_t jobs[MAXJOBS]; /* The job list */
/* End global variables */


/* Function prototypes */

/* Here are the functions that you will implement */
void eval(char *cmdline);
int is_builtin_cmd(char **argv);
void do_exit(void);
void do_show_jobs(void);
void do_ignore_singleton(void);
void do_killall(char **argv);
void do_bgfg(char **argv);
void waitfg(pid_t pid);

void sigchld_handler(int sig);
void sigtstp_handler(int sig);
void sigint_handler(int sig);
void sigalrm_handler(int sig);

/* Here are helper routines that we've provided for you */
int parseline(const char *cmdline, char **argv);
void sigquit_handler(int sig);

void clearjob(struct job_t *job);
void initjobs(struct job_t *jobs);
int maxjid(struct job_t *jobs);
int addjob(struct job_t *jobs, pid_t pid, int state, char *cmdline);
int removejob(struct job_t *jobs, pid_t pid);
pid_t fgpid(struct job_t *jobs);
struct job_t *getprocessid(struct job_t *jobs, pid_t pid);
struct job_t *getjobid(struct job_t *jobs, int jid);
int get_jid_from_pid(pid_t pid);
void showjobs(struct job_t *jobs);

void usage(void);
void unix_error(char *msg);
void app_error(char *msg);
typedef void handler_t(int);
handler_t *Signal(int signum, handler_t *handler);

/*
 * main - The shell's main routine
 */
int main(int argc, char **argv)
{
    char c;
    char cmdline[MAXLINE];
    int emit_prompt = 1; /* emit prompt (default) */

    /* Redirect stderr to stdout (so that driver will get all output
     * on the pipe connected to stdout) */
    dup2(1, 2);

    /* Parse the command line */
    while ((c = getopt(argc, argv, "hvp")) != EOF) {
        switch (c) {
        case 'h':             /* print help message */
            usage();
	    break;
        case 'v':             /* emit additional diagnostic info */
            verbose = 1;
	    break;
        case 'p':             /* don't print a prompt */
            emit_prompt = 0;  /* handy for automatic testing */
	    break;
	default:
            usage();
	}
    }

    /* Install the signal handlers */

    /* These are the ones you will need to implement */
    Signal(SIGINT,  sigint_handler);   /* ctrl-c */
    Signal(SIGTSTP, sigtstp_handler);  /* ctrl-z */
    Signal(SIGCHLD, sigchld_handler);  /* Terminated or stopped child */
    Signal(SIGALRM, sigalrm_handler);  /* Alarm indicates killing all children */

    /* This one provides a clean way to kill the shell */
    Signal(SIGQUIT, sigquit_handler);

    /* Initialize the job list */
    initjobs(jobs);

    /* Execute the shell's read/eval loop */
    while (1) {

	/* Read command line */
	if (emit_prompt) {
	    printf("%s", prompt);
	    fflush(stdout);
	}
	if ((fgets(cmdline, MAXLINE, stdin) == NULL) && ferror(stdin))
	    app_error("fgets error");
	if (feof(stdin)) { /* End of file (ctrl-d) */
	    fflush(stdout);
	    exit(0);
	}

	/* Evaluate the command line */
	eval(cmdline);
	fflush(stdout);
	fflush(stdout);
    }

    exit(0); /* control never reaches here */
}

/*
 * eval - Evaluate the command line that the user has just typed in
 *
 * If the user has requested a built-in command (exit, jobs, bg or fg)
 * then execute it immediately. Otherwise, fork a child process and
 * run the job in the context of the child. If the job is running in
 * the foreground, wait for it to terminate and then return.  Note:
 * each child process must have a unique process group ID so that our
 * background children don't receive SIGINT (SIGTSTP) from the kernel
 * when we type ctrl-c (ctrl-z) at the keyboard.
*/
void eval(char *cmdline) // Parses and interprets the command line
{
  // printf("Entering eval \n");

  pid_t pid; // Unique process ID
  char *argv[MAXARGS]; // Array of chars to hold input
  sigset_t mask; // Set mask to change signals
  int bfg = parseline(cmdline, argv); // Need to parse the input string to see what the command is
  // bfg = 0 if FG job and bfg = 1 if BG job

  if (argv[0] == NULL)   //check if line is empty. Don't do anything.
  {
    return;
  }

  int BLTN = is_builtin_cmd(argv); // Returns type of command

  if (BLTN == 0) // is not a built in command, so need to fork and exec;
  {
    sigemptyset(&mask);
    sigaddset(&mask, SIGCHLD);
    pid = fork();
    if (pid == 0) // Forks off a child process.
    // Within if statement is only child process.
    {
      sigprocmask(SIG_UNBLOCK, &mask, NULL);
      setpgid(0, 0); // Creates a foreground group that will get executed, instead of using the whole foreground.
      if (execv(argv[0], argv) < 0) //Execute child process. If exec command is unsucessful, don't want to run process
      {
          // NOTE: Successful call to exec does not have a return value, it just executes the command
          printf("%s: Command not found \n", argv[0]);
          exit(0);
      }
    }
    // Parent process
    else
    {
      if (bfg == 0) // Perform a Foreground Job
      {
        addjob(jobs, pid, FG, cmdline);
        sigprocmask(SIG_UNBLOCK, &mask, NULL);
        waitfg(pid);
      }
      else // Perfroms a Background job
      {
        addjob(jobs, pid, BG, cmdline);
        sigprocmask(SIG_UNBLOCK, &mask, NULL);
        printf("[%d] (%d) %s", get_jid_from_pid(pid), pid, cmdline);
      }
    }
  }
  else if (BLTN == 1) // is an ignore character command
  {
    do_ignore_singleton();
  }
  else if (BLTN == 2) // is a background or foreground command
  {
    do_bgfg(argv);
  }
  else if (BLTN == 3) // is a jobs command
  {
    do_show_jobs();
  }
  else if (BLTN == 4) // is exit builtin process
  {
    do_exit();
  }
  else if (BLTN == 5) // is a killall command
  {
    do_killall(argv);
  }
  return;
}

/*
 * parseline - Parse the command line and build the argv array.
 *
 * Characters enclosed in single quotes are treated as a single
 * argument.  Return true if the user has requested a BG job, false if
 * the user has requested a FG job.
 */
int parseline(const char *cmdline, char **argv)
{
    // printf("Entering parseline\n");
    static char array[MAXLINE]; /* holds local copy of command line */
    char *buf = array;          /* ptr that traverses command line */
    char *delim;                /* points to first space delimiter */
    int argc;                   /* number of args */
    int bg;                     /* background job? */

    strcpy(buf, cmdline);
    buf[strlen(buf)-1] = ' ';  /* replace trailing '\n' with space */
    while (*buf && (*buf == ' ')) /* ignore leading spaces */
	buf++;

    /* Build the argv list */
    argc = 0;
    if (*buf == '\'') {
	buf++;
	delim = strchr(buf, '\'');
    }
    else {
	delim = strchr(buf, ' ');
    }

    while (delim) {
	argv[argc++] = buf;
	*delim = '\0';
	buf = delim + 1;
	while (*buf && (*buf == ' ')) /* ignore spaces */
	       buf++;

	if (*buf == '\'') {
	    buf++;
	    delim = strchr(buf, '\'');
	}
	else {
	    delim = strchr(buf, ' ');
	}
    }
    argv[argc] = NULL;

    if (argc == 0)  /* ignore blank line */
	return 1;

    /* should the job run in the background? */
    if ((bg = (*argv[argc-1] == '&')) != 0) {
	argv[--argc] = NULL;
    }
    return bg;
}


/*
 * is_builtin_cmd - If the user has typed a built-in command then
 * return the type of built in command, otherwise indicate that it
 * isn't a built in command
 #define BLTN_UNK 0
 #define BLTN_IGNR 1
 #define BLTN_BGFG 2
 #define BLTN_JOBS 3
 #define BLTN_EXIT 4
 #define BLTN_KILLALL 5
 */
int is_builtin_cmd(char **argv) // Compares against exit, fg, bg, jobs, and killall
{
  // printf("Entering is_builtin_cmd\n");
  if ((strcmp(argv[0], "exit") == 0) || (strcmp(argv[0], "quit") == 0)) // strcmp = 0 if strings are the same
  {
    return BLTN_EXIT; // 4
  }
  else if (strcmp(argv[0], "fg") == 0)
  {
    return BLTN_BGFG; // 2
  }
  else if (strcmp(argv[0], "bg") == 0)
  {
    return BLTN_BGFG; // 2
  }
  else if (strcmp(argv[0], "jobs") == 0)
  {
    return BLTN_JOBS; // 3
  }
  else if (strcmp(argv[0], "killall") == 0)
  {
    return BLTN_KILLALL; // 5
  }
  // else if (strcmp(argv[0], '&') == 0)
  // {
  //   return BLTN_IGNR; // 1
  // }
  return BLTN_UNK;     // If it gets here, then not a built in command and returns 0.
}

/*
 * do_exit - Execute the builtin exit command
 */
void do_exit(void)
{
  // printf("Entering do_exit\n");
  exit(0);
}

/*
 * do_show_jobs - Execute the builtin jobs command
 */
void do_show_jobs(void)
{
  // printf("Entering do_show_jobs\n");
  showjobs(jobs);
}

/*
 * do_ignore_singleton - Display the message to ignore a singleton '&'
 */
void do_ignore_singleton(void)
{
  // printf("Entering do_ignore_singleton\n");
  return;
}

void do_killall(char **argv)
{
  // printf("Entering do_killall\n");
  int alarmTime = atoi(argv[1]);
  alarm(alarmTime);
  return;
}

/*
 * do_bgfg - Execute the builtin bg and fg commands
 */
void do_bgfg(char **argv)
{
  // printf("Entering do_bgfg\n");
  int jid; // Job id
  pid_t pid; //program id
  char *params = argv[1]; // Gets the input
  struct job_t *job = NULL; // Make an instance of the job struct for each job called.

  // Check if no parameter, then needs to return
  if (params == NULL)
  {
    printf("%s command requires PID or Jjobid argument\n", argv[0]);
    return;
  }

  //CASE 1: check if the first two characters correspond to the job id
  else if (params[0] == 'J' && isdigit(params[1]))
  {
    jid = atoi(&params[1]); // Get the job id from the input parameters
    job = getjobid(jobs, jid); // Get the job with that job id
    if (job == NULL)
    {
      printf("%s: No such job\n", argv[1]); // If the job doesn't exits, want to return
      return;
    }
  }

  //CASE 2: check if number corresponds to program id
  else if (isdigit(*argv[1]))
  {
    pid = atoi(&params[0]); // Get the process id from input paramters
    job = getprocessid(jobs, pid); // Get the job from the process id
    if (job == NULL) // If there were no jobs with that id found, should return
    {
      printf("(%s): No such process\n", argv[1]);
      return;
    }
  }

  //CASE 3: Then input is invalid
  else
  {
    printf("%s: argument must be a PID or Jjobid\n", argv[0]);
	    return;
  }


  // Now that we have the job, need to move it to bg or fg.
  // CASE 1: Move to background
  if(strcmp("bg", argv[0]) == 0)
  {
    job -> state = BG; // Change the state to background
    printf("[%d] (%d) %s", job->jid, job->pid, job->cmdline);

    // send SIGCONT signal (continue process if stopped) to other processes in group
    // Then terminate the process getting moved
    kill(-job->pid, SIGCONT);
  }
  // CASE 2: Move to foreground
  else if(strcmp("fg", argv[0]) == 0)
  {
    job->state = FG; // Change the state to foreground
    kill(-job->pid, SIGCONT); // Send SIGCONT signal to other processes in group
    waitfg(job->pid); // Then let foreground process finish
  }
  return;
}

/*
 * waitfg - Block other processes until process pid is no longer the foreground process
 */
void waitfg(pid_t pid) // wait for a specific forground process to terminate
{
  // printf("Entering waitfg\n");
  struct job_t *job;
  while (1)
  {
    // Get pid of process
    job = getprocessid(jobs, pid); // Gets the specific job from the pid from the list of jobs
    if (job == NULL || job -> state != FG) // If job exists and is in foreground, sleeps. Otherwise not in foreground so returns.
    {
      return;
    }
    sleep(150); // Sleeps to for some time before checking again
  }
  return;
}

/*****************
 * Signal handlers
 *****************/

/*
 * sigchld_handler - The kernel sends a SIGCHLD to the shell whenever
 *     a child job terminates (becomes a zombie), or stops because it
 *     received a SIGSTOP or SIGTSTP signal. The handler reaps all
 *     available zombie children, but doesn't wait for any other
 *     currently running children to terminate.
 */
void sigchld_handler(int sig)
{
  // printf("Entring sigchld_handler\n");
  int status;
  pid_t pid;
  struct job_t* jid;

  // waitpid(pid, status, options)
  // Allow system to reap zombie processes by suspending
  // executing of the calling process until a child (any child) has changed state

  // waitpid() > 0
  // if OK, returns pid of child that is stopped/terminated,
  // otherwise it returns 0 if WNOHANG, or -1 on error
  // If waitpid = -1, then waits for any child to terminate

  // Options argument is either WNOHANG | WUNTRACED
  // Return value = PID of one of the stopped or terminated children.
  // Return value = 0 then none of the children are stopped or terminated.


  while ((pid = waitpid(-1, &status, WNOHANG | WUNTRACED)) > 0)
  {
    // printf("looping\n");
    jid = getprocessid(jobs, pid);
    if (jid == 0) // Check if there is a job
    {
      return;
    }

    if (WIFSTOPPED(status)) // Checks if child process is currently stopped
    {
      // printf("Entering WIFSTOPPED\n");
      jid = getprocessid(jobs, pid); // Gets job id from joblist
      if (jid == NULL)
      {
        printf("HERE\n");
        return;
      }
      jid->state = ST; // Set state to stopped
      printf("Job [%d] (%d) stopped by signal %d \n", get_jid_from_pid(pid), pid, WSTOPSIG(status)); // WSTOPSIG gets the number of the signal that caused child to stop
    }

    else if (WIFSIGNALED(status)) // Check if child was terminated by a signal
    {
      // printf("Entering WIFSIGNALED\n");
      printf("Job [%d] (%d) terminated by signal %d \n", get_jid_from_pid(pid), pid, WTERMSIG(status));
      removejob(jobs, pid); // delete entires from job list once parent reaps child
    }

    else if (WIFEXITED(status)) // Checks if child was terminated normally
    {
      // printf("Entering WIFEXITED\n");
      removejob(jobs, pid); // delete entries from job list once parent reaps child
    }

    // printf("Ending loop\n");
  }
  // printf("Exiting sigchld_handler\n");
  return;
}

/*
 * sigalrm_handler - The kernel sends a SIGALRM to the shell after
 * alarm(timeout) times out. Catch it and send a SIGINT to every
 * EXISTING (pid != 0) job
 */
void sigalrm_handler(int sig) // Iterates over all jobs and kills the children
{
  // printf("Entering sigalrm_handler\n");
  int i;
  for (i = 0; i < MAXJOBS; i++) // iterate over all jobs
  {
    if (jobs[i].pid != 0) // Check job id
    {
      kill(jobs[i].pid, SIGINT); // Sends SIGINT (kill command) to all commands wiith id != 0
    }
  }
  return;
}

/*
 * sigint_handler - The kernel sends a SIGINT to the shell whenver the
 *    user types ctrl-c at the keyboard.  Catch it and send it along
 *    to the foreground job.
 */
void sigint_handler(int sig)
{
  // printf("Entering sigint_handler\n");
  pid_t pid = fgpid(jobs); // Gets the process id of the processes currently running in the foreground

  if (pid != 0) // Checks to make sure there is a foreground job running
  {
    //printf("[%d](%d) terminated by signal %d\n", get_jid_from_pid(pid), pid, sig);
    kill(-pid, SIGINT); // Sends signal SIGINT, which kills the foreground process group immediately
  }
  return;
}

/*
 * sigtstp_handler - The kernel sends a SIGTSTP to the shell whenever
 *     the user types ctrl-z at the keyboard. Catch it and suspend the
 *     foreground job by sending it a SIGTSTP.
 */
void sigtstp_handler(int sig) // Similar to above function
{
  // printf("Entering sigtstp_handler\n");
  pid_t pid = fgpid(jobs); // Gets the process ID of the foreground process

  if (pid != 0) // Check to see if there is a foreground process before killing anything
  {
    //printf("[%d](%d) Stopped by signal %d\n", get_jid_from_pid(pid), pid, sig);
    kill(-pid, SIGTSTP); // Send SIGTSTP signal, which stops the current foreground process group
  }
  return;
}

/*********************
 * End signal handlers
 *********************/

/***********************************************
 * Helper routines that manipulate the job list
 **********************************************/

/* clearjob - Clear the entries in a job struct */
void clearjob(struct job_t *job) {
    job->pid = 0;
    job->jid = 0;
    job->state = UNDEF;
    job->cmdline[0] = '\0';
}

/* initjobs - Initialize the job list */
void initjobs(struct job_t *jobs) {
    int i;

    for (i = 0; i < MAXJOBS; i++)
	clearjob(&jobs[i]);
}

/* maxjid - Returns largest allocated job ID */
int maxjid(struct job_t *jobs)
{
    int i, max=0;

    for (i = 0; i < MAXJOBS; i++)
	if (jobs[i].jid > max)
	    max = jobs[i].jid;
    return max;
}

/* addjob - Add a job to the job list */
int addjob(struct job_t *jobs, pid_t pid, int state, char *cmdline)
{
    int i;

    if (pid < 1)
	return 0;

    for (i = 0; i < MAXJOBS; i++) {
	if (jobs[i].pid == 0) {
	    jobs[i].pid = pid;
	    jobs[i].state = state;
	    jobs[i].jid = nextjid++;
	    if (nextjid > MAXJOBS)
		nextjid = 1;
	    strcpy(jobs[i].cmdline, cmdline);
  	    if(verbose){
	        printf("Added job [%d] %d %s\n", jobs[i].jid, jobs[i].pid, jobs[i].cmdline);
            }
            return 1;
	}
    }
    printf("Tried to create too many jobs\n");
    return 0;
}

/* removejob - Delete a job whose PID=pid from the job list */
int removejob(struct job_t *jobs, pid_t pid)
{
    int i;

    if (pid < 1)
	return 0;

    for (i = 0; i < MAXJOBS; i++) {
	if (jobs[i].pid == pid) {
	    clearjob(&jobs[i]);
	    nextjid = maxjid(jobs)+1;
	    return 1;
	}
    }
    return 0;
}

/* fgpid - Return PID of current foreground job, 0 if no such job */
pid_t fgpid(struct job_t *jobs) {
    int i;

    for (i = 0; i < MAXJOBS; i++)
	if (jobs[i].state == FG)
	    return jobs[i].pid;
    return 0;
}

/* getprocessid  - Find a job (by PID) on the job list */
struct job_t *getprocessid(struct job_t *jobs, pid_t pid) {
    int i;

    if (pid < 1)
	return NULL;
    for (i = 0; i < MAXJOBS; i++)
	if (jobs[i].pid == pid)
	    return &jobs[i];
    return NULL;
}

/* getjobid  - Find a job (by JID) on the job list */
struct job_t *getjobid(struct job_t *jobs, int jid)
{
    int i;

    if (jid < 1)
	return NULL;
    for (i = 0; i < MAXJOBS; i++)
	if (jobs[i].jid == jid)
	    return &jobs[i];
    return NULL;
}

/* get_jid_from_pid - Map process ID to job ID */
int get_jid_from_pid(pid_t pid)
{
    int i;

    if (pid < 1)
	return 0;
    for (i = 0; i < MAXJOBS; i++)
	if (jobs[i].pid == pid) {
            return jobs[i].jid;
        }
    return 0;
}

/* showjobs - Print the job list */
void showjobs(struct job_t *jobs)
{
    int i;

    for (i = 0; i < MAXJOBS; i++) {
	if (jobs[i].pid != 0) {
	    printf("[%d] (%d) ", jobs[i].jid, jobs[i].pid);
	    switch (jobs[i].state) {
		case BG:
		    printf("Running ");
		    break;
		case FG:
		    printf("Foreground ");
		    break;
		case ST:
		    printf("Stopped ");
		    break;
	    default:
		    printf("showjobs: Internal error: job[%d].state=%d ",
			   i, jobs[i].state);
	    }
	    printf("%s", jobs[i].cmdline);
	}
    }
}
/******************************
 * end job list helper routines
 ******************************/


/***********************
 * Other helper routines
 ***********************/

/*
 * usage - print a help message
 */
void usage(void)
{
    printf("Usage: shell [-hvp]\n");
    printf("   -h   print this message\n");
    printf("   -v   print additional diagnostic information\n");
    printf("   -p   do not emit a command prompt\n");
    exit(1);
}

/*
 * unix_error - unix-style error routine
 */
void unix_error(char *msg)
{
    fprintf(stdout, "%s: %s\n", msg, strerror(errno));
    exit(1);
}

/*
 * app_error - application-style error routine
 */
void app_error(char *msg)
{
    fprintf(stdout, "%s\n", msg);
    exit(1);
}

/*
 * Signal - wrapper for the sigaction function
 */
handler_t *Signal(int signum, handler_t *handler)
{
    struct sigaction action, old_action;

    action.sa_handler = handler;
    sigemptyset(&action.sa_mask); /* block sigs of type being handled */
    action.sa_flags = SA_RESTART; /* restart syscalls if possible */

    if (sigaction(signum, &action, &old_action) < 0)
	unix_error("Signal error");
    return (old_action.sa_handler);
}

/*
 * sigquit_handler - The driver program can gracefully terminate the
 *    child shell by sending it a SIGQUIT signal.
 */
void sigquit_handler(int sig)
{
    printf("Terminating after receipt of SIGQUIT signal\n");
    exit(1);
}
