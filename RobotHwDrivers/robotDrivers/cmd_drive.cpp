#include "cmd_drive.h"
#include "robot.h"

CommandDrive::CommandDrive(int *args){
  left = args[0];
  right = args[1];
}

void CommandDrive::execute(){
	Serial.println("DEBUG,Wheel speed\tLeft: " + String(left) + "\t Right: " + String(right));
	//int16_t intaWheelSpeed[2] = { left, right};
	robot.setMotorSpeed(left,right);  
}

