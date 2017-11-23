/*
 * i2c.cpp
 *
 *  Created on: Nov 22, 2017
 *      Author: daniel
 */
#if 0
#include <Wire.h>



// callback for sending data
// Dont do extra work here.
void sendDatax(){
	int16_t theta;
	int16_t distance_ultra;
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
	case 0x30: //ultra sound sent back if a object is between below the threshold ( value 1)
	  distance_ultra = robot.readUltraSound();
    if(distance_ultra < 40){
      Wire.write(1);
    }
    else{
      Wire.write(0);
    }
		break;
	case 0x55:
		//Wire.write(203);
		break;
	}
//Wire.write(buf,len);
//Wire.write(buf[0]);
}


// callback for received data
void receiveDatax(int byteCount){
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
    case 0x30: // request Ultrasound
      break;
    case 0x40: //turn on buzzer with a frequency. state0=NoTone state1=1Khz state2=2Khz tone
      robot.buzzer(buf[1]);
      break;
    }
  }
}



I2C::I2C()
	: I2C(sendDatax, receiveDatax){
}


I2C::I2C( void(*sendData)(), void(*receiveData)(int))
: sendData(sendData)
, receiveData(receiveData){

	// initialize i2c as slave
	Wire.begin(SLAVE_ADDRESS);

	// define callbacks for i2c communication
	Wire.onReceive(receiveData);
	Wire.onRequest(sendData);
}
#endif
