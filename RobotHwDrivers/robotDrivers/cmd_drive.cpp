#include "cmd_drive.h"
#include "pin_header.h"

void driveWheels(int v[2]);
void leftWheel(int motorPower);
void rightWheel(int motorPower);

CommandDrive::CommandDrive(int *args)
{
  left = args[0];
  right = args[1];
}

void CommandDrive::execute(){
  Serial.println("DEBUG,Wheel speed\tLeft: " + String(left) + "\t Right: " + String(right));
  int intaWheelSpeed[2] = {left, right};
  driveWheels(intaWheelSpeed);
}

void driveWheels(int v[2]){
  // v[0] = leftWheel     v[1] = rightWheel
  leftWheel(v[0]);
  rightWheel(v[1]);
}

void leftWheel(int motorPower){
    motorPower = constrain(motorPower, -255, 255);   // constrain motorPower to -255 to +255
    if(motorPower <= 0){ // spin CCW
        digitalWrite(L_CTRL1, HIGH);
        digitalWrite(L_CTRL2, LOW);
        analogWrite(L_PWM, abs(motorPower));
    }else{ // spin CW
        digitalWrite(L_CTRL1, LOW);
        digitalWrite(L_CTRL2, HIGH);
        analogWrite(L_PWM, abs(motorPower));
    }
}

void rightWheel(int motorPower){
    motorPower = constrain(motorPower, -255, 255);   // constrain motorPower to -255 to +255
    if(motorPower <= 0){  // spin CCW
        digitalWrite(R_CTRL1, HIGH);
        digitalWrite(R_CTRL2, LOW);
        analogWrite(R_PWM, abs(motorPower));
    }else{ // spin CW
        digitalWrite(R_CTRL1, LOW);
        digitalWrite(R_CTRL2, HIGH);
        analogWrite(R_PWM, abs(motorPower));
    }
}
