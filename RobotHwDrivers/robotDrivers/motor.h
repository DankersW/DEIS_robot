// motor.h

#ifndef _MOTOR_h
#define _MOTOR_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "Arduino.h"
#else
	#include "WProgram.h"
#endif

class Motor
{
 protected:
	enum State{
		IDLE = 0,
		RUNNING = 1
	};

#if 0
	int16_t speed_l;
	int16_t speed_r;

	State	state_l;
	State	state_r; 
#endif
	int16_t speed;
	State state;

	// pins
	uint8_t ctrl1;
	uint8_t ctrl2;
	uint8_t pwm;
	void set(int16_t power);
	void brake();
	//void setWheel(int motor_power, int16_t ctrl1, int16_t ctrl2,int16_t pwm, State wheel_state, uint8_t wheel);
	//void breakWheel(uint8_t ctrl1, uint8_t ctrl2);

 public:
	Motor(uint8_t ctrl1, uint8_t ctrl2, uint8_t pwm);
	void setSpeed(int16_t l);
	void stop();

};

#endif

