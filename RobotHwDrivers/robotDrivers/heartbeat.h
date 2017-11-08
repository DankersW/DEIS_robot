// heartbeat.h

#ifndef _HEARTBEAT_H_
#define _HEARTBEAT_H_
#include "types.h"

#if defined(ARDUINO) && ARDUINO >= 100
	#include "Arduino.h"
#else
	#include "WProgram.h"
#endif

class Heartbeat
{
 protected:
	// heartbeat period in millis
	static const uint64_t HEARBEAT_PERIOD = 1000; 

	uint64_t time_last_message;
	
 public:
	Heartbeat();
	void update(int distance, encoder_t wheel_enc);
};

//extern Heartbeat h;

#endif

