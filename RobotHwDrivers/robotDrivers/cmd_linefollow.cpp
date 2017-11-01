#include "cmd_linefollow.h"
#include "pin_header.h"
#include "Redbot.h"

static void driveWheels(int v[2]);
static void leftWheel(int motorPower);
static void rightWheel(int motorPower);
static void getIRvalues(int r[3]);

//RedBotSensor IRSensor0 = RedBotSensor(A3); // initialize a IRsensor object on A3 ~left
//RedBotSensor IRSensor1 = RedBotSensor(A6); // initialize a IRsensor object on A6 ~middle
//RedBotSensor IRSensor2 = RedBotSensor(A7); // initialize a IRsensor object on A7 ~right

CommandLinefollow::CommandLinefollow(int *args)
{
  SPEED = args[0];
}

void CommandLinefollow::execute(){
  int V_left; 
  int V_right;
  //int intaWheelSpeed[2] = {0,0};
  int irValues[3] = {0};
  while (!Serial.available()){
    getIRvalues(irValues);

    if(irValues[0] > LINETHRESHOLD){ //left
      V_left  = SPEED;
      V_right = SPEED + SPEED;
    }
    else if(irValues[1] > LINETHRESHOLD){ //middle
      V_left  = SPEED; 
      V_right = SPEED;
    }
    else if(irValues[2] > LINETHRESHOLD){ //right
      V_left  = SPEED + SPEED;
      V_right = SPEED;
    }
    
    int intaWheelSpeed[2] = {V_left,V_right};
    driveWheels(intaWheelSpeed);
  } 
}

static void driveWheels(int v[2]){
  // v[0] = leftWheel     v[1] = rightWheel
  leftWheel(v[0]);
  rightWheel(v[1]);
}

static void leftWheel(int motorPower){
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

static void rightWheel(int motorPower){
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

static void getIRvalues(int r[3]){
  // saves the IR values into the array of the arugment r[0]=LeftIR   r[1]=MiddleIR   r[2]=RightIR
  r[0] = IRSensor0.read();
  r[1] = IRSensor1.read();
  r[2] = IRSensor2.read();
}
