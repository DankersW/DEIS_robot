// 
// 
// 

#include "robot.h"
#include "pin_header.h"
#include "controller.h"

//unsigned int pingSpeed = 50; // How frequently are we going to send out a ping (in milliseconds). 50ms would be 20 times a second.
//unsigned long pingTimerUltra;     // Holds the next ping time.
//int ultra_distance;
//NewPing sonar(TRIGGER_PIN, ECHO_PIN, 200);

Robot::Robot()
	: encoder (RedBotEncoder(A2, 10))// initializes encoder on pins A2 and 10
	, left_motor(Motor(L_CTRL1,L_CTRL2,L_PWM))
	, right_motor(Motor(R_CTRL1,R_CTRL2,R_PWM))
	, left_line_sensor(RedBotSensor(A3))// initialize a IRsensor object on A3 ~left
	, middle_line_sensor(RedBotSensor(A6)) // initialize a IRsensor object on A6 ~middle
	, right_line_sensor(RedBotSensor(A7))  // initialize a IRsensor object on A7 ~right
{
  // setup Ultra sound
  //pinMode(TRIGGER, OUTPUT);
  //pinMode(ECHO, INPUT);
  //digitalWrite(TRIGGER, 0);

  //ultra sound time interuppt
  
  //pingTimerUltra = millis(); // Start now.

  //setup buzzer pin
  pinMode(11, OUTPUT);
}

void Robot::stop(){
	left_motor.stop();
	right_motor.stop();
}

//void Robot::echoCheck() { // Timer2 interrupt calls this function every 24uS where you can check the ping status.
//  if (sonar.check_timer()) { // This is how you check to see if the ping was received.
//    ultra_distance =  sonar.ping_result / US_ROUNDTRIP_CM;
//  }
//}

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
  static int distance;
  long duration;
  static long trigger_start = -1;

  return 100;
  //if (millis() >= pingTimerUltra) {   // pingSpeed milliseconds since last ping, do another ping.
    //pingTimerUltra += pingSpeed;      // Set the next ping time.
    //sonar.ping_timer(Robot::echoCheck); // Send out the ping, calls "echoCheck" function every 24uS where you can check the ping status.   
  //}
  //return ultra_distance;
  /*
  if(trigger_start < 0){
	  digitalWrite(TRIGGER, HIGH);
	  trigger_start = millis();
  }
  else if(trigger_start + 1 >= millis()){
	  //delayMicroseconds(20); // needed for correct readings
	  trigger_start = -1;
	  digitalWrite(TRIGGER, LOW);
	  duration = pulseIn(ECHO, HIGH);
	  distance = (duration/22) / 29.1;
  }
  */
  /*
  if (millis() >= pingTimer) {   // pingSpeed milliseconds since last ping, do another ping.
    pingTimer += pingSpeed;      // Set the next ping time.
    sonar.ping_timer(echoCheck); // Send out the ping, calls "echoCheck" function every 24uS where you can check the ping status.
  }

  
  //distance
  int temp = sonar.ping_cm();
  if(temp != 0)
	  distance = temp;
  return distance;
  */
}
/*
void echoCheck() { // Timer2 interrupt calls this function every 24uS where you can check the ping status.
  // Don't do anything here!
  if (sonar.check_timer()) { // This is how you check to see if the ping was received.
    // Here's where you can add code.
    Serial.print("Ping: ");
    Serial.print(sonar.ping_result / US_ROUNDTRIP_CM); // Ping returned, uS result in ping_result, convert to cm with US_ROUNDTRIP_CM.
    Serial.println("cm");
  }
  // Don't do anything here!
}
*/

void Robot::buzzer(int state){
#if 0
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
#endif
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

