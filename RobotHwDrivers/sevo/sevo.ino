/* Sweep
 by BARRAGAN <http://barraganstudio.com>
 This example code is in the public domain.

 modified 8 Nov 2013
 by Scott Fitzgerald
 http://www.arduino.cc/en/Tutorial/Sweep
*/

#include <Servo.h>

Servo servoRight;  // create servo object to control a servo
Servo servoLeft;  // create servo object to control a servo
// twelve servo objects can be created on most boards

int pos = 0;    // variable to store the servo position

void setup() {
  servoRight.attach(9);  // attaches the servo on pin 9 to the servo object
  servoLeft.attach(3); 

  servoRight.write(180);  
  servoLeft.write(0);
  
}

void loop() {
  delay(1000);
  angleScoop(0);
  delay(3000);
  angleScoop(1);
  delay(3000);
  angleScoop(2);
  delay(3000);
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

