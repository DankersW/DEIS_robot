// 
// 
// 

#include "robot.h"
#include "pin_header.h"
#include "controller.h"
Robot::Robot()
	: encoder (RedBotEncoder(A2, 10))// initializes encoder on pins A2 and 10
	, motor(Motor())
	, left_line_sensor(RedBotSensor(A3))// initialize a IRsensor object on A3 ~left
	, middle_line_sensor(RedBotSensor(A6)) // initialize a IRsensor object on A6 ~middle
	, right_line_sensor(RedBotSensor(A7))  // initialize a IRsensor object on A7 ~right
	

{
	servo_right.attach(9);
	servo_left.attach(3);
}




void Robot::stop(){
	motor.stop();
}

void Robot::angleScoop(int mode){
	// 0 = left   1 = right  2 = both
	// 180° closes    30° open
	switch (mode){
	case 0: // other robot is on the left side
		servo_right.write(30);
		servo_left.write(0);
		break;
	case 1: // other robot is on the right side
		servo_right.write(180);
		servo_left.write(150);
		break;
	case 2: // robots on both sides
		servo_right.write(180);
		servo_left.write(0);
		break;
	default:
		break;
	}
}


encoder_t Robot::readWheelEncoders(){
	encoder_t vals;
	vals.left		= encoder.getTicks(LEFT);
	vals.right		= encoder.getTicks(RIGHT);
	return vals;
}

line_sensors_t Robot::readLineSensors(){
	line_sensors_t linevals;
	linevals.left	= left_line_sensor.read();
	linevals.middle = middle_line_sensor.read();
	linevals.right	= right_line_sensor.read();
	return linevals;
}
void Robot::readSensors(Controller c){
	line_sensors_t linevals;
	encoder_t velocity;
	encoder_t vals;
	
	vals = readWheelEncoders();
	linevals = readLineSensors();
	
	
	//- TODO: Move this out
	
	//c.updateLineSensors(linevals);
	//velocity = c.update(vals);
	
	Serial.println("\tleftEnc: "+ String(vals.left) + "\trightEnc: " + String(vals.right) + "\tIR L: " + String(linevals.left) + "\tIR M: " + String(linevals.middle) + "\tIR R: " + String(linevals.right) + "\tleft V: " + String(velocity.left) + "\tRight V: " + String(velocity.right));

#ifdef TODO
	if(velocity.left == 0 && velocity.right == 0 ){
		CommandStop commandStop = CommandStop(0);
		commandStop.execute();
		}else{
		int intaWheelSpeed[2] = {int(velocity.left), int(velocity.right)};
		driveWheels(intaWheelSpeed);
	}
#endif
	
}


void Robot::setMotorSpeed(int16_t left, int16_t right){
	motor.setSpeed(left,right);
}

void Robot::clearWheelEncoders(){
	encoder.clearEnc(BOTH);  // Reset the counters.
}


Robot robot = Robot();

