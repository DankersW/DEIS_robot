// 
// 
// 

#include "heartbeat.h"

Heartbeat::Heartbeat()
	: time_last_message(millis()) { // initialize to current time
}

void Heartbeat::update(int distance){
	unsigned long now = millis();
  String message = "";
	if(now - time_last_message >= HEARBEAT_PERIOD){
		// TODO: Send message here
		// Possibly change conditions
   if(distance < 60) //object detected less then 60 cm from robot
      message += "US, " + String(distance);
      Serial.println(message);
	}
}

//Heartbeat Heartbeat;

