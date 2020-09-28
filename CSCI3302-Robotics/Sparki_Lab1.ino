#include <Sparki.h>

#define STATE_ZERO 0
#define STATE_ONE 1
#define STATE_TWO 2
#define STATE_THREE 3
#define STATE_FOUR 4
#define STATE_FIVE 5
#define STATE_SIX 6
#define STATE_SEVEN 7
#define STATE_EIGHT 8
#define STATE_NINE 9
int cur_state = 0;
int threshold = 300;

void setup() {
  // put your setup code here, to run once:
}

//void loop() {
//  if (cur_state == STATE_ZERO) {
//    sparki.moveRight();
//  }
//  int cm = sparki.ping();
//  if(cm != -1){
//    if (cm <= 30) {
//      cur_state = STATE_ONE;
//      //sparki.moveForward();
//    }
//  }
//  if (cur_state == STATE_ONE) {
//    sparki.moveForward();
//    if(sparki.ping() <= 7 && sparki.ping() != -1){
//      cur_state = STATE_TWO;
//    }
//  }
//  if(cur_state == STATE_TWO){
//    sparki.moveForward(5);
//    cur_state = STATE_THREE;
//  }
//  if(cur_state == STATE_THREE){
//    sparki.gripperClose();
//    delay(3000);
//    sparki.gripperStop();
//    delay(1500);
//    cur_state = STATE_FOUR;
//  }
//  if(cur_state == STATE_FOUR){
//    sparki.moveRight(180);
//    cur_state = STATE_FIVE;
//  }
//  if(cur_state == STATE_FIVE){
//    sparki.gripperOpen(10);
//    delay(1500);
//    sparki.moveBackward(10);
//  }
//  delay(100);
//}

void loop() 
{
  int cm = sparki.ping();
  switch(cur_state) 
  {
    case(STATE_ZERO): // Rotate clockwise until it detects on object.
    {
      sparki.moveRight();
      if ((cm <= 30) && (cm > 0))
      {
        cur_state = STATE_ONE;
      }
      break;
    }
    case(STATE_ONE): // Move forward until it detects an object.
    {
      sparki.moveForward();
      if ((cm <= 7) && (cm > 0))
      {
        cur_state = STATE_TWO;
      }
      break;
    }
    case(STATE_TWO): // Move forward a little extra to make sure it gets the object.
    {
      sparki.moveForward(5);
      cur_state = STATE_THREE;
      break;
    }
    case(STATE_THREE): // Close grippper on object.
    {
      sparki.gripperClose();
      delay(3000);
      sparki.gripperStop();
      delay(1500);
      cur_state = STATE_FOUR;
      break;
    }
    case(STATE_FOUR): // Rotate 180 degrees.
    {
      sparki.moveRight(180);
      cur_state = STATE_FIVE;
      break;
    }
    case(STATE_FIVE): // Move forward until it detects a line.
    {
      sparki.moveForward();
      int lineLeft = sparki.lineLeft();
      int lineCenter = sparki.lineCenter();
      int lineRight = sparki.lineRight();
      if (lineLeft < threshold || lineRight < threshold || lineCenter < threshold)
      {
        cur_state = STATE_SIX;
      }
      break;
    }
    case (STATE_SIX): // Move along the line until it detects the start line.
    {
      int lineLeft   = sparki.lineLeft();   // measure the left IR sensor
      int lineCenter = sparki.lineCenter(); // measure the center IR sensor
      int lineRight  = sparki.lineRight();  // measure the right IR sensor
      if ( lineLeft < threshold ) // if line is below left line sensor
      {  
        sparki.moveLeft(); // turn left
      }
     
      if ( lineRight < threshold ) // if line is below right line sensor
      {  
        sparki.moveRight(); // turn right
      }
     
      // if the center line sensor is the only one reading a line
      if ( (lineCenter < threshold) && (lineLeft > threshold) && (lineRight > threshold) )
      {
        sparki.moveForward(); // move forward
      }  
      if (lineLeft < threshold && lineRight < threshold && lineCenter < threshold)
      {
        sparki.moveStop();
        cur_state = STATE_SEVEN;
      }
      delay(100); // wait 0.1 seconds
      break;
    }
    case (STATE_SEVEN):
    {
      sparki.beep();
      cur_state = STATE_EIGHT;
      break;
    }
    case (STATE_EIGHT): // Don't do anything
    {
      sparki.gripperOpen(5);
      delay(1500);
      cur_state = STATE_NINE;
      break;
    }
    case (STATE_NINE): // Don't move
    {
      sparki.moveStop();
    }
    default: // In case it doesn't have a state
    {
      printf("Something else");
      sparki.clearLCD(); // wipe the screen
      sparki.updateLCD(); // display all of the information written to the screen
      break;
    }
  }
  delay(100);
}

