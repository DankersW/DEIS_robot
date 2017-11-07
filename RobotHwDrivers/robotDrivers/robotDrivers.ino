#include <RedBot.h>
#include "command.h"
#include "command_factory.h"
#include "pin_header.h"
#include "controller.h"
#include "robot.h"
#include "heartbeat.h"

Heartbeat heart_beat = Heartbeat();
uint64_t last_lane_change;
bool right = true;
void setup() {
	Serial.begin(9600);
	Serial.setTimeout(0);


	pinMode(12, INPUT_PULLUP); //onboard button

	while (!Serial); // wait for serial port to connect. Needed for native USB port only
	robot.angleScoop(0);
  //delay(1000);
  //robot.angleScoop(1);
  //delay(1000);
  //robot.angleScoop(2);
	//controller.startLineFollow(70);
}


static void readSerial(){
	static String serial_input = "";
	int data = 0;
	
	if(Serial.available()){
		data = Serial.read();

		if(data == '\n'){  // newline received on the channel
			Command *cmd = CommandCreator::parse(serial_input);
			cmd->execute();

			Serial.println("DEBUG,serialInput: |" + String(serial_input) + "|");
			
			serial_input = "";
			delete cmd;
		}
		else{
			serial_input += char(data);
		}
	}
}

void loop() {
	readSerial();

	//Serial.println("Test");
	// read sensors
	encoder_t		    wheel_enc		   = robot.readWheelEncoders();
	line_sensors_t	line_sensors	 = robot.readLineSensors();
  int             objectDistance = robot.readUltraSound();
  
	// update controller
	encoder_t speeds = controller.update(wheel_enc, line_sensors, objectDistance);
  
	//Serial.println("Left speed: " + String(speeds.left) + " right speed: " + String(speeds.right));
 
	// update hardware
	robot.setMotorSpeed(speeds.left, speeds.right);
 
	// send heartbeat to python
	heart_beat.update(objectDistance);
}

	controller.startLineFollow(70);
	last_lane_change = millis();
  robot.angleScoop(2);
	if((millis() - last_lane_change) > 5000 ){
		controller.startLaneChange(right);
		right = !right;
		last_lane_change = millis();
	}
	//Serial.println("Test");