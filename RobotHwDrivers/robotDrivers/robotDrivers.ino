#include <RedBot.h>
#include "command.h"
#include "command_factory.h"
#include "pin_header.h"
#include "controller.h"
#include "robot.h"
#include "heartbeat.h"
#include "Wire.h"


#define SLAVE_ADDRESS 0x04

Heartbeat heart_beat = Heartbeat();
uint64_t last_lane_change;
bool right = true;
uint8_t buf[20];
uint8_t len;
bool data_available = false;



// callback for received data
void receiveData(int byteCount){
	uint8_t index = 0;
	//Serial.print(byteCount);
	while(byteCount--){
		data_available = true;
		buf[index++] = Wire.read();
		//Serial.println(buf[index-1]);
	}
	len = index;
//Serial.println("len = " + String(len));
	data_available = true;
}

// callback for sending data
// Dont do extra work here.
void sendData(){
	int16_t theta;
	int16_t x;
	int16_t y;
	pos_t p;
	//Serial.print("Sending cmd :");
	//Serial.println(buf[0]);
	switch(buf[0]){
	case 1:
		Wire.write(0);
		break;
	case 0x10:
		//Wire.beginTransmission();
		p = controller.getPosition();
		Serial.println("Sending x");
		Wire.write(p.x_array, sizeof(p.x_array));
		break;
	case 0x12:
		p = controller.getPosition();
		Serial.println("Sending y: " + String(p.y));
		Wire.write(p.y_array, sizeof(p.y_array));
		break;
	case 0x14:
		p = controller.getPosition();
		uint8_t buf[2];
		theta = (int16_t)(p.theta*1000);

		buf[0] = theta & 0xFF;
		buf[1] = (theta >> 8)&0xFF;
		Wire.write(buf, sizeof(buf));
		break;
	case 0x20:
		//Wire.write(201);
		break;
	case 202:
		//Wire.write(202);
		break;
	case 203:
		//Wire.write(203);
		break;
	}
//Wire.write(buf,len);
//Wire.write(buf[0]);
}

void readData(){
  int16_t theta;
  
  if(data_available){
    int16_t x;
    int16_t y;
    Serial.println("Cmd: " + String(buf[0]));
    data_available = false;
    switch(buf[0]){
    case 1: // scoop commmand
      robot.angleScoop(buf[1]);
      break;
    case 0x10:
      if(len < 3){
        Serial.println("Reg: " + String(buf[0]) + " not enough data. Len = " + String(len));
        break;
      }
      x = buf[1] | buf[2] << 8;
      Serial.println("Setting x to: " + String(x));
      controller.position.x = x;
      break;
    case 0x12:
      if(len < 3){
        Serial.println("Reg: " + String(buf[0]) + " Not enough data");
        break;
      }
      y = buf[1] | buf[2] << 8;
      Serial.println("Setting y to: " + String(y));
      controller.position.y = y;
      break;
    case 0x14:
      if(len < 3){
        Serial.println("Reg: " + String(buf[0]) + "Not enough data");
        break;
      }
      theta = buf[1] | buf[2] << 8;
      controller.position.theta = ((double)theta) / 1000;
      Serial.println("Theta set to: " + String(controller.position.theta));
      break;
    case 0x20: //lanefollow with speed
      controller.startLineFollow(buf[1]);
      break;
    case 0x22: //lane change with direction
      controller.startLaneChange(buf[1]==1, 35);
      break;
    case 0x30:
      break;
    }
    case 0x40: //turn on buzzer with a frequentie. state0=NoTone state1=1Khz state2=2Khz tone
      robot.buzzer(buf[1]);
  }
}



void setup() {
	Serial.begin(9600);
	Serial.setTimeout(0);

	// initialize i2c as slave
	Wire.begin(SLAVE_ADDRESS);

	// define callbacks for i2c communication
	Wire.onReceive(receiveData);
	Wire.onRequest(sendData);


	pinMode(12, INPUT_PULLUP); //onboard button

	while (!Serial); // wait for serial port to connect. Needed for native USB port only
	robot.angleScoop(2);
  
	controller.startLineFollow(80); //
	//controller.startLaneChange(true, 35);
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
  // read data/commands from PI
	readData();

 
	// read sensors
	encoder_t		      wheel_enc	 	    = robot.readWheelEncoders();
	line_sensors_t		line_sensors	  = robot.readLineSensors();
	int16_t           object_distance = robot.readUltraSound();
  
	// update controller
	encoder_t speeds = controller.update(wheel_enc, line_sensors, object_distance);
  
	//Serial.println("distance: " + String(object_distance) + " Left speed: " + String(speeds.left) + " right speed: " + String(speeds.right));
 
	// update hardware
	robot.setMotorSpeed(speeds.left, speeds.right);
 
	// send heartbeat to python
	//heart_beat.update(object_distance);
}

