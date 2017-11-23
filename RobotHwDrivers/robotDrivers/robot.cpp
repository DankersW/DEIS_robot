// 
// 
// 

#include "robot.h"
#include "pin_header.h"
#include "controller.h"
Robot::Robot()
	: encoder (RedBotEncoder(A2, 10))// initializes encoder on pins A2 and 10
	, left_motor(Motor(L_CTRL1,L_CTRL2,L_PWM))
	, right_motor(Motor(R_CTRL1,R_CTRL2,R_PWM))
	, left_line_sensor(RedBotSensor(A3))// initialize a IRsensor object on A3 ~left
	, middle_line_sensor(RedBotSensor(A6)) // initialize a IRsensor object on A6 ~middle
	, right_line_sensor(RedBotSensor(A7))  // initialize a IRsensor object on A7 ~right

{
  // setup Ultra sound
  pinMode(TRIGGER, OUTPUT);
  pinMode(ECHO, INPUT);
  digitalWrite(TRIGGER, 0);

  //setup buzzer pin
  pinMode(11, OUTPUT);
}

void Robot::stop(){
	left_motor.stop();
	right_motor.stop();
}

void Robot::angleScoop(int mode){
	servo_right.attach(9);
	servo_left.attach(3);

	// 0 = left   1 = right  2 = both
	// 180 closes    30 open
	switch (mode){
	case 0: // other robot is on the left side
    Serial.println("Debug,Robot::angleScoop, Setting mode 0");
		servo_right.write(30);
		servo_left.write(0);
		break;
	case 1: // other robot is on the right side
    Serial.println("Debug,Robot::angleScoop, Setting mode 1");
		servo_right.write(180);
		servo_left.write(150);
		break;
	case 2: // robots on both sides
    Serial.println("Debug,Robot::angleScoop, Setting mode 2");
		servo_right.write(180);
		servo_left.write(0);
		break;
	default:
		break;
	}
}
int Robot::readUltraSound(){
  int distance; 
  long duration;
  
  digitalWrite(TRIGGER, HIGH);
  delayMicroseconds(20); // needed for correct readings
  digitalWrite(TRIGGER, LOW);
  duration = pulseIn(ECHO, HIGH);
  distance = (duration/2) / 29.1;
  
  return distance;
}

void Robot::buzzer(int state){
  noTone(11);
  if(state == 0){
    noTone(11);
  }
  else if(state == 1){
    tone(11, 1000);  
  }
  else if(state == 2){
    tone(11, 2000);  
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
	
	//Serial.println("\tleftEnc: "+ String(vals.left) + "\trightEnc: " + String(vals.right) + "\tIR L: " + String(linevals.left) + "\tIR M: " + String(linevals.middle) + "\tIR R: " + String(linevals.right) + "\tleft V: " + String(velocity.left) + "\tRight V: " + String(velocity.right));

}


void Robot::setMotorSpeed(int16_t left, int16_t right){
	left_motor.setSpeed(left);
	right_motor.setSpeed(right);
	//motor.setSpeed(left,right);
}

void Robot::clearWheelEncoders(){
	encoder.clearEnc(BOTH);  // Reset the counters.
}

encoder_t Robot::getWheelSpeeds(){
	return {right_motor.getSpeed(), left_motor.getSpeed() };
}

Robot robot = Robot();

