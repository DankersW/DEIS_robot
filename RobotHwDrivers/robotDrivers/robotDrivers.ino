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
	robot.angleScoop(2);
	//delay(1000);
	//robot.angleScoop(1);
	//delay(1000);
	//robot.angleScoop(2);
	//controller.startLineFollow(70);
	//controller.startLineFollow(70);
	last_lane_change = millis();
  
	controller.startLineFollow(70); //
	controller.startLaneChange(true, 35);
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

	if((millis() - last_lane_change) > 15000 ){
		controller.startLaneChange(right,  50);
		right = !right;
		last_lane_change = millis();
	}
	//Serial.println("Test");
	// read sensors
	encoder_t		    wheel_enc	 	= robot.readWheelEncoders();
	line_sensors_t		line_sensors	= robot.readLineSensors();
	int16_t             object_distance = robot.readUltraSound();
  
	// update controller
	encoder_t speeds = controller.update(wheel_enc, line_sensors, object_distance);
  
	//Serial.println("distance: " + String(object_distance) + " Left speed: " + String(speeds.left) + " right speed: " + String(speeds.right));
 
	// update hardware
	robot.setMotorSpeed(speeds.left, speeds.right);
 
	// send heartbeat to python
	heart_beat.update(object_distance);
}

