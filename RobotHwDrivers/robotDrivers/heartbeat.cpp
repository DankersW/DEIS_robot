// 
// 
// 

#include "heartbeat.h"

Heartbeat::Heartbeat()
	: time_last_message(millis()) { // initialize to current time
}

void Heartbeat::update(){
	unsigned long now = millis();
	if(now - time_last_message >= HEARBEAT_PERIOD){
		// TODO: Send message here
		// Possibly change conditions
	}
}

//Heartbeat Heartbeat;

