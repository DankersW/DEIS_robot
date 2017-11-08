#include "motor.h"
#include "pin_header.h"

Motor::Motor(uint8_t ctrl1, uint8_t ctrl2, uint8_t pwm)
	: speed(0)
	, state(IDLE)
	, ctrl1(ctrl1)
	, ctrl2(ctrl2)
	, pwm(pwm)
	//speed_l(0)
	//, speed_r(0)
	//, state_l(IDLE)
	//, state_r(IDLE)
{

	pinMode(ctrl1, OUTPUT);
	pinMode(ctrl2, OUTPUT);
	pinMode(pwm, OUTPUT);
	//left motor
#if 0
	pinMode(L_CTRL1, OUTPUT);
	pinMode(L_CTRL2, OUTPUT);
	pinMode(L_PWM, OUTPUT);

	//right motor
	pinMode(R_CTRL1, OUTPUT);
	pinMode(R_CTRL2, OUTPUT);
	pinMode(R_PWM, OUTPUT);
#endif
}


// Only allows forward motion
void Motor::setSpeed(int16_t power){
	if(power > 0)
		set(power);
	else
		brake();
	state = power == 0 ? IDLE: RUNNING;
	speed = power;
}

void Motor::stop(){
	setSpeed(0);
}

void Motor::set(int16_t power){
	power = constrain(power, -255,255);
	if(state == IDLE){
		digitalWrite(ctrl1, LOW);
		digitalWrite(ctrl2, HIGH);
	}
	analogWrite(pwm, abs(power));
}

void Motor::brake(){
	// setting both controls HIGH, shorts the motor out -- causing it to self brake
	digitalWrite(ctrl1, HIGH);
	digitalWrite(ctrl2, HIGH);
	analogWrite(pwm, 0);
}

int16_t Motor::getSpeed(){
	return speed;
}
