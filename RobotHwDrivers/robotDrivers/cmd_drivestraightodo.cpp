#include "cmd_drivestraightodo.h"
#include "pin_header.h"
#include "controller.h"
#include <RedBot.h>
#include "cmd_stop.h"

static void driveWheels(int v[2]);
static void leftWheel(int motorPower);
static void rightWheel(int motorPower);

CommandDriveStraightOdo::CommandDriveStraightOdo(int *args)
{
  SPEED = args[0];
  DISTANCE = args[1];
}

void CommandDriveStraightOdo::execute(){
  //(pos_t start = {0}, encoder_t encoders = {0}
  Serial.println("DEBUG,Drive straight ODO: \tSPEED: " + String(SPEED) + "\t DISTANCE: " + String(DISTANCE));

  cmd_input_t inputVal;
  inputVal.distance = DISTANCE;
  inputVal.speedOg = SPEED;
  
  controller.setWaypoint(inputVal);
  Serial.println("DISTANCE INIT :" + String(inputVal.distance));

  encoder.clearEnc(BOTH);  // Reset the counters.
  
  while (!Serial.available()){
    encoder_t vals;
    line_sensors_t linevals;
    
    
    vals.left = encoder.getTicks(LEFT);
    vals.right = encoder.getTicks(RIGHT);
    linevals.left = IRSensor0.read();
    linevals.middle = IRSensor1.read();
    linevals.right = IRSensor2.read();
  
    controller.updateLineSensors(linevals);
    encoder_t velocity = controller.update(vals);
    
    Serial.println("\tleftEnc: "+ String(vals.left) + "\trightEnc: " + String(vals.right) + "\tIR L: " + String(linevals.left) + "\tIR M: " + String(linevals.middle) + "\tIR R: " + String(linevals.right) + "\tleft V: " + String(velocity.left) + "\tRight V: " + String(velocity.right));

    if(velocity.left == 0 && velocity.right == 0 ){
      CommandStop commandStop = CommandStop(0);
      commandStop.execute();
    }else{
      int intaWheelSpeed[2] = {int(velocity.left), int(velocity.right)};
      driveWheels(intaWheelSpeed); 
    }
  }

  encoder.clearEnc(BOTH);  // Reset the counters.
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

