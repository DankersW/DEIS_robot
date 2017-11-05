#include "heartbeat.h"
#include <RedBot.h>
#include "cmds/command.h"
#include "cmds/command_factory.h"
#include "pin_header.h"
#include "controller.h"
#include "robot.h"

static String serial_input = "";
//Controller controller = Controller();

void setup() {
	Serial.begin(9600);
	Serial.setTimeout(0);

	pinMode(12, INPUT_PULLUP); //onboard button
	
	//left motor
	pinMode(L_CTRL1, OUTPUT);
	pinMode(L_CTRL2, OUTPUT);
	pinMode(L_PWM, OUTPUT);
	
	//right motor
	pinMode(R_CTRL1, OUTPUT);
	pinMode(R_CTRL2, OUTPUT);
	pinMode(R_PWM, OUTPUT);

	while (!Serial); // wait for serial port to connect. Needed for native USB port only
}


static void readSerial(){
	Command *cmd;
	
	int data = 0;
	
	if(Serial.available()){
		data = Serial.read();
		
		
		if(data == '\n'){  // newline received on the channel
			
			Serial.println("DEBUG,serialInput: |" + String(serial_input) + "|");
			cmd = CommandCreator::parse(serial_input);
			cmd->execute();
			//Serial.println("pos actually: " + String(pos_x) + ", " + String(pos_y));
			
			delete cmd;
			serial_input = "";
		}
		else{
			serial_input += char(data);
		}
	}
}

void loop() {
	readSerial();
	
	encoder_t		wheel_enc		= robot.readWheelEncoders();
	line_sensors_t	line_sensors	= robot.readLineSensors();
	
	controller.update(wheel_enc, line_sensors);
	
}


//void sendSensorData(){
//  int encoder[2] = {0};
//  int irValues[3] = {0};
//  getEncoderValues(encoder);
//  //getIRvalues(irValues);
//
//  String output = "All," + String(encoder[0]) + "," + String(encoder[1]) + ","  + String(irValues[0]) + "," + String(irValues[1]) + "," + String(irValues[2]) ;
//  Serial.flush();
//  Serial.println(output);
//}






