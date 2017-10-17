#include "cmd_stop.h"
#include "pin_header.h"
void stopWheels();
void leftWheelBreak();
void rightWheelBreak();


CommandStop::CommandStop(int *args)
{
  // nothing to do
}

void CommandStop::execute(){
  Serial.println("DEBUG,Stopping");
  stopWheels();
  while (1==1){
    asm("nop");
  }
}

void stopWheels(){
  leftWheelBreak();
  rightWheelBreak();
}

void leftWheelBreak(){
    // setting both controls HIGH, shorts the motor out -- causing it to self brake.
    digitalWrite(L_CTRL1, HIGH);
    digitalWrite(L_CTRL2, HIGH);
    analogWrite(L_PWM, 0);
}

void rightWheelBreak(){  
    // setting both controls HIGH, shorts the motor out -- causing it to self brake.
    digitalWrite(L_CTRL1, HIGH);
    digitalWrite(L_CTRL2, HIGH);
    analogWrite(R_PWM, 0);
}


