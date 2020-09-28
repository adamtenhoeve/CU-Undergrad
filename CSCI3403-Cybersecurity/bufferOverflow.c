int main (int argc, char *argv[]) {
	int valid = FALSE;
	char str1[8];
	char str2[8];
  next_tag(str1);
  fgets(str2, 8);
  if (strcomp(str1, str2, 8) == 0)
    valid = TRUE;
  printf("buffer1: str1(%s), str2(%s), valid(%d)\n", str1, str2, valid);
}

void hello(char *tag)
{
  char inp[16];
  printf("Enter value for %s: ", tag);
  fgets(inp, 16);
  printf("Hello your %s is %s\n", tag, inp);
}

void getinp(char *inp, int siz)
{
  puts("Input value: ");
  fgets(inp, siz, stdin);
  printf("buffer3 getinp read %s\n", inp);
}
void display(char *val)
{
  char tmp[16];
  snprintf(tmp, 27, "read val: %s\n", val);
  puts(tmp);
}
int main(int argc, char *argv)
{
  char buf[16];
  getinp(buf, sizeof(buf));
  display(buf);
  printf("buffer3 done\n");
}
