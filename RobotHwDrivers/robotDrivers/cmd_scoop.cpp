#include "cmd_scoop.h"
//#include "pin_header.h"
#include <Servo.h>

void angleScoop(int postionOtherRobots);

// create servo object
Servo servoRight;  
Servo servoLeft;


CommandScoop::CommandScoop(int *args)
{
  mode = args[0];
}

void CommandScoop::execute(){
  
  servoRight.attach(9);  
  servoLeft.attach(3);
  
  Serial.println("Scoop mode:" + String(mode));
  angleScoop(mode);
}

void angleScoop(int postionOtherRobots){
  // 0 = left   1 = right  2 = both 
  // 180° closes    30° open
  switch (postionOtherRobots){
    case 0: // other robot is on the left side
      servoRight.write(30);  
      servoLeft.write(0);
      break;
    case 1: // other robot is on the right side
      servoRight.write(180);  
      servoLeft.write(150);
      break;
    case 2: // robots on both sides
      servoRight.write(180);  
      servoLeft.write(0);
      break;
    default:
      break;
  }
}

