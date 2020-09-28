#include <sparki.h>
#include <math.h>

#define M_PI 3.14159
#define ROBOT_SPEED 0.0278 // meters/second
#define CYCLE_TIME .100 // Default 50ms cycle time
#define AXLE_DIAMETER 0.0857 // meters
#define WHEEL_RADIUS 0.03 // meters
#define CONTROLLER_FOLLOW_LINE 1
#define CONTROLLER_GOTO_POSITION_PART2 2
#define CONTROLLER_GOTO_POSITION_PART3 3

#define FWD 1
#define NONE 0
#define BCK -1


// Line following configuration variables
const int threshold = 700;
int line_left = 1000;
int line_center = 1000;
int line_right = 1000;
int last_command = -1;
// Controller and dTheta update rule settings
const int current_state = CONTROLLER_GOTO_POSITION_PART3;

// Odometry bookkeeping
float orig_dist_to_goal = 0.0;
float pose_x = 0., pose_y = 0., pose_theta = 0.;
float dest_pose_x = 0., dest_pose_y = 0., dest_pose_theta = 0.;
float d_err = 0., b_err = 0., h_err = 0.; // Distance error (m), bearing error (rad), heading error (rad)
float phi_l = 0., phi_r = 0.; // Wheel rotation (radians)


// Wheel rotation vars
float left_speed_pct = 0.;
float right_speed_pct = 0.;
int left_dir = DIR_CCW;
int right_dir = DIR_CW;
int left_wheel_rotating = NONE;
int right_wheel_rotating = NONE;

// X and Theta Updates (global for debug output purposes)
// and their respective feedback controller gains
const float distance_gain = 1.;
const float theta_gain = 1.;
float dX  = 0., dTheta = 0.;

float to_radians(double deg) {
  return  deg * 3.1415/180.;
}

float to_degrees(double rad) {
  return  rad * 180 / 3.1415;
}

void setup() {
  pose_x = 0.;
  pose_y = 0.;
  pose_theta = 0.;
  left_wheel_rotating = NONE;
  right_wheel_rotating = NONE;

  // Set test cases here!
  set_pose_destination(.15, .30, to_radians(180));  // Goal_X_Meters, Goal_Y_Meters, Goal_Theta_Degrees
}

// Sets target robot pose to (x,y,t) in units of meters (x,y) and radians (t)
void set_pose_destination(float x, float y, float t) {
  dest_pose_x = x;
  dest_pose_y = y;
  dest_pose_theta = t;
  if (dest_pose_theta > M_PI) dest_pose_theta -= 2*M_PI;
  if (dest_pose_theta < -M_PI) dest_pose_theta += 2*M_PI;
  orig_dist_to_goal = 0; // TODO
}

void readSensors() {
  line_left = sparki.lineLeft();
  line_right = sparki.lineRight();
  line_center = sparki.lineCenter();
}


//void updateOdometry() {
//  float dist = ROBOT_SPEED * 0.1; 
//  switch(last_command){
//    case 1:
//      pose_theta += (dist/AXLE_DIAMETER) + (dist/AXLE_DIAMETER); //right wheel/distance - ((-left wheel)/distance)
//      break;
//    case 2:
//      pose_theta += (-dist/AXLE_DIAMETER) - (dist/AXLE_DIAMETER); //((-right wheel)/distance) - ((left wheel)/distance)
//      break;
//    case 3:
//      pose_theta += (dist/AXLE_DIAMETER) - (dist/AXLE_DIAMETER); //((right wheel)/distance) - ((left wheel)/distance)
//      pose_x += cos(pose_theta) * dist; 
//      pose_y += sin(pose_theta) * dist;
//      break;
//  }
//  // Bound theta
//  if (pose_theta > M_PI) pose_theta -= 2.*M_PI;
//  if (pose_theta <= -M_PI) pose_theta += 2.*M_PI;
//}

void updateOdometry() {
  pose_theta += ((CYCLE_TIME * ROBOT_SPEED * right_speed_pct)/AXLE_DIAMETER) - ((CYCLE_TIME * ROBOT_SPEED * left_speed_pct)/AXLE_DIAMETER); //right wheel/distance - ((-left wheel)/distance)
  pose_x += cos(pose_theta) * ((CYCLE_TIME * ROBOT_SPEED * right_speed_pct / 2.0) + ((CYCLE_TIME * ROBOT_SPEED * left_speed_pct / 2.0))); 
  pose_y += sin(pose_theta) * ((CYCLE_TIME * ROBOT_SPEED * right_speed_pct / 2.0) + ((CYCLE_TIME * ROBOT_SPEED * left_speed_pct / 2.0)));
  // Bound theta
  if (pose_theta > M_PI) pose_theta -= 2.*M_PI;
  if (pose_theta <= -M_PI) pose_theta += 2.*M_PI;
}

void displayOdometry() {
  sparki.print("X: ");
  sparki.print(pose_x);
  sparki.print(" Xg: ");
  sparki.println(dest_pose_x);
  sparki.print("Y: ");
  sparki.print(pose_y);
  sparki.print(" Yg: ");
  sparki.println(dest_pose_y); 
  sparki.print("T: ");
  sparki.print(to_degrees(pose_theta));
  sparki.print(" Tg: ");
  sparki.println(to_degrees(dest_pose_theta));

  sparki.print("dX : ");
  sparki.print(dX );
  sparki.print("   dT: ");
  sparki.println(dTheta);
  sparki.print("phl: "); sparki.print(phi_l); sparki.print(" phr: "); sparki.println(phi_r);
  sparki.print("p: "); sparki.print(d_err); sparki.print(" a: "); sparki.println(to_degrees(b_err));
  sparki.print("h: "); sparki.println(to_degrees(h_err));  
}

void loop() {
  unsigned long begin_time = millis();
  unsigned long end_time = 0;
  unsigned long delay_time = 0;
//  if(last_command != -1){
//    updateOdometry();
//    displayOdometry();
//  }
  switch (current_state) {
    case CONTROLLER_FOLLOW_LINE:
      // Useful for testing odometry updates
      readSensors();
      if (line_center < threshold) {
          last_command = 3;
          sparki.moveForward();
      } else if (line_left < threshold) {
          last_command = 1; 
          sparki.moveLeft();
      } else if (line_right < threshold) {
          last_command = 2;
          sparki.moveRight();
      } 

      // Check for start line, use as loop closure
      if (line_left < threshold && line_right < threshold && line_center < threshold) {
        pose_x = 0.;
        pose_y = 0.;
        pose_theta = 0.;
      } 
      break;
    case CONTROLLER_GOTO_POSITION_PART2:
      // TODO: Implement solution using moveLeft, moveForward, moveRight functions
      // This case should arrest control of the program's control flow (taking as long as it needs to, ignoring the 100ms loop time)
      // and move the robot to its final destination

      d_err = sqrt(pow(pose_x - dest_pose_x, 2) + pow(pose_y - dest_pose_y, 2));
      b_err = atan2((dest_pose_y - pose_y),(dest_pose_x - pose_x)) - pose_theta;
      h_err = dest_pose_theta - pose_theta;

      sparki.moveLeft(to_degrees(b_err));
      sparki.moveForward(d_err);
      sparki.moveLeft(to_degrees(h_err - b_err));
      while(true)
        sparki.moveStop();      

    
      break;      
    case CONTROLLER_GOTO_POSITION_PART3:      
      // TODO: Implement solution using motorRotate and proportional feedback controller.
      // sparki.motorRotate function calls for reference:
      //      sparki.motorRotate(MOTOR_LEFT, left_dir, int(left_speed_pct*100));
      //      sparki.motorRotate(MOTOR_RIGHT, right_dir, int(right_speed_pct*100));

      d_err = sqrt(pow(pose_x - dest_pose_x, 2.0) + pow(pose_y - dest_pose_y, 2.0));
      b_err = atan2((dest_pose_y - pose_y),(dest_pose_x - pose_x)) - pose_theta;
      h_err = dest_pose_theta - pose_theta;

      // If distance (d_err) is greater than 10cm, ignore the heading error.

      if (d_err < 0.04 && b_err < to_radians(5) && h_err < to_radians(5))
      {
        while(1) sparki.moveStop();
      }

      if (d_err > 0.04)
      {
        phi_l = ( 0.3 * d_err - (b_err * AXLE_DIAMETER / 2.0)) / WHEEL_RADIUS; // Measured in radians
        phi_r = ( 0.3 * d_err + (b_err * AXLE_DIAMETER / 2.0)) / WHEEL_RADIUS;
          
//          phi_l = ( 0.3 * d_err - ((1 * b_err + 0.01 * h_err) * AXLE_DIAMETER / 2.0)) / WHEEL_RADIUS;
//          phi_r = ( 0.3 * d_err + ((1 * b_err + 0.01 * h_err) * AXLE_DIAMETER / 2.0)) / WHEEL_RADIUS;
      }
      else
      {
          phi_l = ( 0 * d_err - ((0 * b_err + h_err) * AXLE_DIAMETER / 2.0)) / WHEEL_RADIUS; // Measured in radians
          phi_r = ( 0 * d_err + ((0 * b_err + h_err) * AXLE_DIAMETER / 2.0)) / WHEEL_RADIUS;
      }
      left_speed_pct = phi_l / max(abs(phi_l), phi_r);
      right_speed_pct = phi_r / max(abs(phi_l), phi_r);

      updateOdometry();

      if(phi_l < 0){
        sparki.motorRotate(MOTOR_LEFT, right_dir, int(abs(left_speed_pct) * 100));
      } else {
        sparki.motorRotate(MOTOR_LEFT, left_dir, int(abs(left_speed_pct) * 100));
      }
      sparki.motorRotate(MOTOR_RIGHT, right_dir, int(right_speed_pct * 100));
  }

  sparki.clearLCD();
  displayOdometry();
  sparki.updateLCD();

  end_time = millis();
  delay_time = end_time - begin_time;
  if (delay_time < 1000*CYCLE_TIME)
    delay(1000*CYCLE_TIME - delay_time); // each loop takes CYCLE_TIME ms
  else
    delay(10);
}

