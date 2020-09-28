#include <sparki.h>

#define CYCLE_TIME .100  // seconds

// Program States
#define CONTROLLER_FOLLOW_LINE 1
#define CONTROLLER_DISTANCE_MEASURE 2


int current_state = CONTROLLER_FOLLOW_LINE; // Change this variable to determine which controller to run
const int threshold = 600;
int line_left = 1000;
int line_center = 1000;
int line_right = 1000;
float pose_x = 0., pose_y = 0., pose_theta = 0.;
unsigned long time; 
unsigned long time2;
const float speed = (30.0/100.0) /(10773.0/1000.0); //speed in m/s
const float distance_wheels = 0.0857;
int last_command = -1;
void setup() {
  pose_x = 0.;
  pose_y = 0.;
  pose_theta = 0.;
  Serial.begin(9600);
}

void readSensors() {
  line_left = sparki.lineLeft();
  line_right = sparki.lineRight();
  line_center = sparki.lineCenter();
  //distance = sparki.ping();
}

void measure_30cm_speed() {
  //None
}


void updateOdometry() {
  float dist = speed * 0.1; 
  switch(last_command){
    case 1:
      pose_theta += (dist/distance_wheels) + (dist/distance_wheels); //right wheel/distance - ((-left wheel)/distance)
      break;
    case 2:
      pose_theta += (-dist/distance_wheels) - (dist/distance_wheels); //((-right wheel)/distance) - ((left wheel)/distance)
      break;
    case 3:
      pose_theta += (dist/distance_wheels) - (dist/distance_wheels); //((right wheel)/distance) - ((left wheel)/distance)
      pose_x += cos(pose_theta) * dist; 
      pose_y += sin(pose_theta) * dist;
      break;
  }
  
}

void displayOdometry() {
  sparki.clearLCD();
  sparki.println(last_command);
  sparki.print("X pos: ");
  sparki.println(pose_x);
  sparki.print("Y pos: ");
  sparki.println(pose_y);
  sparki.print("Theta: ");
  sparki.println(pose_theta);
  sparki.updateLCD();
}

void loop() { 
   time = millis();
   if(last_command != -1){
      updateOdometry();
      displayOdometry();
   }
  // TODO: Insert loop timing/initialization code here
  
  switch (current_state) {
    case CONTROLLER_FOLLOW_LINE: 
        readSensors();
        if (line_left < threshold ) // if line is below left line sensor
        {  
            sparki.moveLeft(); // turn left
            last_command = 1; //Right is pos, left is neg
        }
 
        if ( line_right < threshold ) // if line is below right line sensor
        {  
          sparki.moveRight(); // turn right
          last_command = 2; //Left is pos, right is neg
        }
       
        // if the center line sensor is the only one reading a line
        if ( (line_center < threshold) && (line_left > threshold) && (line_right > threshold) )
        {
          sparki.moveForward(); // move forward
          last_command = 3; //Both pos 
        }  
        if ( (line_center < threshold) && (line_left < threshold) && (line_right < threshold) )
        {
          pose_x = 0.;
          pose_y = 0.;
          pose_theta = 0.;
          sparki.moveForward(); // move forward
          //last_command = 3; //Both pos
        }  
      break;
    case CONTROLLER_DISTANCE_MEASURE:
      measure_30cm_speed();
      break;
  }
  time2 = millis();
  if(time2 - time > 100){
    sparki.beep();
  }
  delay(100 - (time2 - time));
}
