// UPDATED 10/10
#include <sparki.h>

#define ROBOT_SPEED 0.0278
#define MIN_CYCLE_TIME .100
#define AXLE_DIAMETER 0.0865
#define FWD 1
#define NONE 0
#define BCK -1

// Screen size
#define SCREEN_X_RES 128.0
#define SCREEN_Y_RES 64.0

// Map size
#define NUM_X_CELLS 4
#define NUM_Y_CELLS 4

// Start line is 18", 2" from bottom left corner
#define START_LINE_X .4572
#define START_LINE_Y .0508

#define SERVO_POS_DEG 45

int current_state = 1;
const int threshold = 800;
int line_left = 1000;
int line_center = 1000;
int line_right = 1000;
float distance = 0.;
unsigned long last_cycle_time = 0;

float pose_x = 0., pose_y = 0., pose_theta = 0., pose_servo = 0.;
int left_wheel_rotating = 0, right_wheel_rotating = 0;

// TODO: Define world_map multi-dimensional array

bool world_map[NUM_X_CELLS][NUM_Y_CELLS];

// TODO: Figure out how many meters of space are in each grid cell
// 4 cells, 60cm, so each is 15cm long
const float CELL_RESOLUTION_X = 0.6 / NUM_X_CELLS;  // Line following map is ~60cm x ~42cm
// 4 cells, 42cm wide, so 10.5cm wide
const float CELL_RESOLUTION_Y = 0.42 / NUM_Y_CELLS;// Line following map is ~60cm x ~42cm


void setup() {
  pose_x = START_LINE_X;
  pose_y = START_LINE_Y;
  pose_theta = 0.;
  left_wheel_rotating = NONE;
  right_wheel_rotating = NONE;
  pose_servo = to_radians(SERVO_POS_DEG);

  sparki.servo(-to_degrees(pose_servo)); // Servo axis points down instead of up, so we need to negate it to be consistent with the robot's rotation axis

  // TODO: Initialize world_map
  int i,j;
  for(i = 0; i < NUM_X_CELLS; i++)
  {
    for(j = 0; j < NUM_Y_CELLS; j++)
    {
      world_map[i][j] = false; // False means nothing, True means obstacle
    }
  }

  sparki.clearLCD();
  displayMap();
  delay(1000);
  last_cycle_time = millis();
}

float to_radians(float deg) {
  return deg * 3.14159 / 180.;
}

float to_degrees(float rad) {
  return rad * 180. / 3.14159;
}

// Ultrasonic Sensor Readings -> Robot coordinates
void transform_us_to_robot_coords(float dist, float theta, float *rx, float *ry) {
  *rx = dist*cos(theta);
  *ry = dist*sin(theta);
}

// Robot coordinates -> World frame coordinates
void transform_robot_to_world_coords(float x, float y, float *gx, float *gy) {
  *gx = (cos(pose_theta)*x - sin(pose_theta)* y) + pose_x;
  *gy = (sin(pose_theta)*x + cos(pose_theta)* y) + pose_y;
}

bool transform_xy_to_grid_coords(float x, float y, int *i, int *j) {
  // TODO: Set *i and *j to their corresponding grid cords
  if ((x > 0) && (x <= 0.6) && (y > 0) && (y <= 0.42))
  {
    *i = int(x / CELL_RESOLUTION_X); // What row it is in
    *j = int(y / CELL_RESOLUTION_Y); // What column it is in
  }
  // TODO: Return 0 if the X,Y coordinates were out of bounds
  else
  {
    return 0;
  }
  return 1;
}

// Turns grid coordinates into world coordinates (grid centers)
bool transform_grid_coords_to_xy(int i, int j, float *x, float *y) {
  // TODO: Return 0 if the I,J coordinates were out of bounds
  if ((i > 0) && (i <= 4) && (j > 0) && (j <= 4))
  {
    *x = i * CELL_RESOLUTION_X + 0.5 * CELL_RESOLUTION_X;
    *y = j * CELL_RESOLUTION_Y + 0.5 * CELL_RESOLUTION_Y;
  }
  // TODO: Set *x and *y
  else
  {
    return 0;
  }
  return 1;
}

void readSensors() {
  line_left = sparki.lineLeft();
  line_right = sparki.lineRight();
  line_center = sparki.lineCenter();
  distance = float(sparki.ping()) / 100.;
}

void moveRight() {
  left_wheel_rotating = FWD;
  right_wheel_rotating = BCK;
  sparki.moveRight();
}

void moveLeft() {
  left_wheel_rotating = BCK;
  right_wheel_rotating = FWD;
  sparki.moveLeft();
}

void moveForward() {
  left_wheel_rotating = FWD;
  right_wheel_rotating = FWD;
  sparki.moveForward();
}

void moveBackward() {
  left_wheel_rotating = BCK;
  right_wheel_rotating = BCK;
  sparki.moveBackward();
}

void moveStop() {
  left_wheel_rotating = NONE;
  right_wheel_rotating = NONE;
  sparki.moveStop();
}

void updateOdometry(float cycle_time) {
  pose_x += cos(pose_theta) * cycle_time * ROBOT_SPEED * (left_wheel_rotating + right_wheel_rotating)/2.;
  pose_y += sin(pose_theta) * cycle_time * ROBOT_SPEED * (left_wheel_rotating + right_wheel_rotating)/2.;
  pose_theta += (right_wheel_rotating - left_wheel_rotating) * cycle_time * ROBOT_SPEED / AXLE_DIAMETER;
}

void displayMap() {
  // TODO: Measure how many pixels will be taken by each grid cell
  const int PIXELS_PER_X_CELL = 32; // Screen is 128, 4 cells, so each is 32 pixels
  const int PIXELS_PER_Y_CELL = 16;  // Screen res = 64, 4 cell, so each is 16 pixels
  int cur_cell_x=-1, cur_cell_y = -1;
  // TODO: Make sure that if the robot is "off-grid", e.g., at a negative grid position or somewhere outside your grid's max x or y position that you don't try to plot the robot's position!
  if (transform_xy_to_grid_coords(pose_x, pose_y, &cur_cell_x, &cur_cell_y) != 0)
  {
    sparki.clearLCD();
//    sparki.drawCircle(3,3,3);
//    sparki.updateLCD();
    int i,j;
    for (i = 0; i < NUM_X_CELLS; i++)
    {
      for (j = 0; j < NUM_Y_CELLS; j++)
      {
        if (i == cur_cell_x && j == cur_cell_y)// Sparki location
        {
          sparki.drawCircle((PIXELS_PER_X_CELL * i) + (0.5 * PIXELS_PER_X_CELL), ((PIXELS_PER_Y_CELL * (NUM_Y_CELLS - j - 1)) + (0.5 * PIXELS_PER_Y_CELL)), 4);
        }
        else if (world_map[i][j] == true) // detects object
        {
          sparki.drawCircleFilled((PIXELS_PER_X_CELL * i) + (0.5 * PIXELS_PER_X_CELL), (PIXELS_PER_Y_CELL * (NUM_Y_CELLS - j - 1)) + (0.5 * PIXELS_PER_Y_CELL), 7);
        }
        else{
          sparki.drawCircle((PIXELS_PER_X_CELL * i) + (0.5 * PIXELS_PER_X_CELL), ((PIXELS_PER_Y_CELL * (NUM_Y_CELLS - j - 1)) + (0.5 * PIXELS_PER_Y_CELL)), 7);
        }
      }
    }
    sparki.updateLCD();
  }
}

void serialPrintOdometry() {
  Serial.print("\n\n\nPose: ");
  Serial.print("\nX: ");
  Serial.print(pose_x);
  Serial.print("\nY: ");
  Serial.print(pose_y);
  Serial.print("\nT: ");
  Serial.print(pose_theta * 180. / 3.14159);
  Serial.print("\n");
}

void displayOdometry() {
  sparki.println("Pose: ");
  sparki.print("X: ");
  sparki.println(pose_x);
  sparki.print("Y: ");
  sparki.println(pose_y);
  sparki.print("T: ");
  sparki.println(pose_theta * 180. / 3.14159);
}

void loop() {
  unsigned long begin_time = millis();
  unsigned long begin_movement_time = 0;
  unsigned long end_time = 0;
  unsigned long delay_time = 0;
  float elapsed_time = 0.;
  bool found_object = 0;
  readSensors();

  sparki.clearLCD();
  last_cycle_time = (millis() - last_cycle_time);
  elapsed_time = last_cycle_time / 1000.;
  updateOdometry(elapsed_time);
  last_cycle_time = millis(); // Start timer for last motor command to determine cycle time
  serialPrintOdometry();

  // Mapping Code
  sparki.servo(-to_degrees(pose_servo));

  // TODO: Check if sensors found an object

  int objectI, objectJ;
  float objectX, objectY;
  objectI = -1;
  objectJ = -1;

  if (distance > 0 && distance < 0.2)
  {
    transform_us_to_robot_coords(distance, pose_servo, &objectX, &objectY);
    transform_robot_to_world_coords(objectX, objectY, &objectX, &objectY);
    transform_xy_to_grid_coords(objectX, objectY, &objectI, &objectJ);
    if (objectI != -1 && objectJ != -1)
    {
      world_map[objectI][objectJ] = true;
    }
  }

  // TODO: Adjust Map to accommodate new object

  displayMap();

  if (line_center < threshold) {
    moveForward();
  } else if (line_left < threshold) {
    moveLeft();
  } else if (line_right < threshold) {
    moveRight();
  } else {
    moveStop();
  }

  // Check for start line, use as loop closure
  // NOTE: Assumes robot is moving counter-clockwise around the map (by setting pose_theta = 0)!
  //       If your robot is moving clockwise, set pose_theta to pi radians (i.e., pointing left).
  if (line_left < threshold && line_right < threshold && line_center < threshold) {
    pose_x = START_LINE_X;
    pose_y = START_LINE_Y;
    pose_theta = 0.;
  }

  end_time = millis();
  delay_time = end_time - begin_time;
  if (delay_time < 1000*MIN_CYCLE_TIME)
    delay(1000*MIN_CYCLE_TIME - delay_time); // make sure each loop takes at least MIN_CYCLE_TIME ms
  else
    delay(10);
}













int GridtoNumber(int i, int j)
{
  int width;
  width = 4;
  return (j * width + i);
}

void NumbertoGrid(int n, int *i, int *j)
{
  int width;
  width = 4;
  *i = 0;
  *j = 0;

  while (n > width)
  {
    *j++;
    n = n - width;
  }
  *i = n;
}

int CostFunction(int num1, int num2)
{
//  int distArray[NUM_X_CELLS][NUM_Y_CELLS];
//  int i, j;
//  for (i = 0; i < NUM_X_CELLS; i++)
//  {
//    for (j = 0; j < NUM_Y_CELLS; j++)
//    {
//      distArray[i][j] = 99;
//    }
//  }
//
//  int i1, j1, i2, j2, index1, index2;
//  NumbertoGrid(num1, &i1, &j1);
//  NumbertoGrid(num2, &i2, &j2);
//
//  distArray[i1][j1] = 0; // Initialize initial spot of array
//
//  for (index1 = 0; index1 < NUM_X_CELLS; index1++)
//  {
//    for (index2 = 0; index < NUM_Y_CELLS; index2++)
//    {
//      // if adjacent is unobstructed, increase distance and set last value
//      // otherwise ignore that block
//      if (world_map[index1][index2] == false)
//      {
//
//      }
//    }
//  }

  int i1, j1, i2, j2, index1, index2;
  NumbertoGrid(num1, &i1, &j1);
  NumbertoGrid(num2, &i2, &j2);

  if ((abs(i1-i2) == 1 && (j1-j2 == 0)) || ((i1 - i2 == 0) && (abs(j1-j2) == 1)))
  {
    return 1;
  }
  return 99;
}
