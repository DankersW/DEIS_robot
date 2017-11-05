#if AS
#include <RedBot.h>
#include "cmds/command.h"
#include "cmds/command_factory.h"
#include "pin_header.h"
#include "controller.h"
#include "robot.h"
#include "heartbeat.h"

Heartbeat heart_beat = Heartbeat();
void setup() {
	Serial.begin(9600);
	Serial.setTimeout(0);

	pinMode(12, INPUT_PULLUP); //onboard button

	while (!Serial); // wait for serial port to connect. Needed for native USB port only
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

	// read sensors
	encoder_t		wheel_enc		= robot.readWheelEncoders();
	line_sensors_t	line_sensors	= robot.readLineSensors();
	
	// update controller
	controller.update(wheel_enc, line_sensors);

	// send heartbeat to python
	heart_beat.update();

	
}

#endif
