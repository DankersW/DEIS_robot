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
}



#if 0
robot.clearWheelEncoders();

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

robot.clearWheelEncoders();

#endif
