#include "cmd_stop.h"
#include "pin_header.h"

static void stopWheels();
static void leftWheelBreak();
static void rightWheelBreak();


CommandStop::CommandStop(int *args)
{
  // nothing to do
}

void CommandStop::execute(){
  Serial.println("DEBUG,Stopping");
  stopWheels();
  //while (1==1){
    //asm("nop");
  //}
}

static void stopWheels(){
  leftWheelBreak();
  rightWheelBreak();
}

static void leftWheelBreak(){
    // setting both controls HIGH, shorts the motor out -- causing it to self brake.
    digitalWrite(L_CTRL1, HIGH);
    digitalWrite(L_CTRL2, HIGH);
    analogWrite(L_PWM, 0);
}

static void rightWheelBreak(){  
    // setting both controls HIGH, shorts the motor out -- causing it to self brake.
    digitalWrite(L_CTRL1, HIGH);
    digitalWrite(L_CTRL2, HIGH);
    analogWrite(R_PWM, 0);
}


