// robot.h

#ifndef _ROBOT_H_
#define _ROBOT_H_
#include <Servo.h>
#include <RedBot.h>
#include "types.h"
#include "motor.h"
class Controller;


#if defined(ARDUINO) && ARDUINO >= 100
	#include "Arduino.h"
#else
	#include "WProgram.h"
#endif

class Robot
{
	
 protected:
	Servo servo_right;
	Servo servo_left;
	
	RedBotEncoder encoder;
	
	RedBotSensor left_line_sensor;
	RedBotSensor middle_line_sensor;
	RedBotSensor right_line_sensor;
	Motor motor;
	
 public:
	line_sensors_t readLineSensors();
	encoder_t readWheelEncoders();
	void angleScoop(int mode);
	void setMotorSpeed(int16_t left, int16_t right);
	
	void stop();
	void clearWheelEncoders();
	void readSensors(Controller c);
	Robot();

};

extern Robot robot;

#endif

