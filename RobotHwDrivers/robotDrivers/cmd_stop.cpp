#include "cmd_stop.h"
#include "pin_header.h"
#include "robot.h"
static void stopWheels();
static void leftWheelBreak();
static void rightWheelBreak();


CommandStop::CommandStop(int *args)
{
  // nothing to do
}

void CommandStop::execute(){
  Serial.println("DEBUG,Stopping");
  
  robot.stop();
}




#if 0
static void leftWheelBreak(){
    // setting both controls HIGH, shorts the motor out -- causing it to self brake.
    digitalWrite(L_CTRL1, HIGH);
    digitalWrite(L_CTRL2, HIGH);
    analogWrite(L_PWM, 0);
}

static void rightWheelBreak(){  
    // setting both controls HIGH, shorts the motor out -- causing it to self brake.
    digitalWrite(R_CTRL1, HIGH);
    digitalWrite(R_CTRL2, HIGH);
    analogWrite(R_PWM, 0);
}
#endif
