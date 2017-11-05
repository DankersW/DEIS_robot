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

	int16_t speed_l;
	int16_t speed_r;

	State	state_l;
	State	state_r; 
	
	void setWheel(int motor_power, int16_t ctrl1, int16_t ctrl2, State wheel_state);
	void breakWheel(uint8_t ctrl1, uint8_t ctrl2);

 public:
	Motor();
	void setSpeed(int16_t l, int16_t r);
	void stop();

};

#endif

