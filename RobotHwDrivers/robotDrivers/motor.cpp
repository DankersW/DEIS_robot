#include "motor.h"
#include "pin_header.h"

Motor::Motor()
	: state_l(IDLE)
	, state_r(IDLE)
	, speed_l(0)
	, speed_r(0)
{
	//left motor
	pinMode(L_CTRL1, OUTPUT);
	pinMode(L_CTRL2, OUTPUT);
	pinMode(L_PWM, OUTPUT);

	//right motor
	pinMode(R_CTRL1, OUTPUT);
	pinMode(R_CTRL2, OUTPUT);
	pinMode(R_PWM, OUTPUT);
}


// Only allows forward motion
void Motor::setSpeed(int16_t l, int16_t r){
	if(l > 0) 
		setWheel(l, L_CTRL1, L_CTRL2,state_l);
	else
		breakWheel(L_CTRL1, L_CTRL2);// shorts
	
	if(r > 0)
		setWheel(r, R_CTRL1, R_CTRL2, state_r);
	else
		breakWheel(R_CTRL1, R_CTRL2);// shorts
	
	state_l = l == 0 ? IDLE : RUNNING;
	state_r = r == 0 ? IDLE : RUNNING;
	
	speed_l = l;
	speed_r = r;
}
void Motor::stop(){
	setSpeed(0,0);
}

void Motor::setWheel(int motor_power, int16_t ctrl1, int16_t ctrl2, State state){
	motor_power = constrain(motor_power, -255, 255);   // constrain motorPower to -255 to +255
	if(state == IDLE){
		digitalWrite(ctrl1, LOW);
		digitalWrite(ctrl2, HIGH);
	}
	analogWrite(R_PWM, abs(motor_power));
}
#if 0
void Motor::setWheel(int motor_power, int16_t ctrl1, int16_t ctrl2){
	motor_power = constrain(motor_power, -255, 255);   // constrain motorPower to -255 to +255
	if(motor_power <= 0){  // spin CCW
		digitalWrite(ctrl1, HIGH);
		digitalWrite(ctrl2, LOW);
		analogWrite(R_PWM, abs(motor_power));
		}else{ // spin CW
		digitalWrite(ctrl1, LOW);
		digitalWrite(ctrl2, HIGH);
		analogWrite(R_PWM, abs(motor_power));
	}
}
#endif
void Motor::breakWheel(uint8_t ctrl1, uint8_t ctrl2){
	// setting both controls HIGH, shorts the motor out -- causing it to self brake
	digitalWrite(ctrl1, HIGH);
	digitalWrite(ctrl2, HIGH);
	analogWrite(L_PWM, 0);
}
