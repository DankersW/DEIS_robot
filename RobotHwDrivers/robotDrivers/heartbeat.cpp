// 
// 
// 

#include "heartbeat.h"
#include "types.h"
#include "controller.h"

Heartbeat::Heartbeat()
	: time_last_message(millis()) { // initialize to current time
}

void Heartbeat::update(int distance){
	unsigned long now = millis();
  String message = "";
	if(now - time_last_message >= HEARBEAT_PERIOD){
    //Serial.println(now - time_last_message);
    time_last_message = now;
		// TODO: Send message here
		// Possibly change conditions
    if(distance < 50){ //object detected less then 60 cm from robot
      message = "US, " + String(distance);
      Serial.println(message);
    }
	  
   
    pos_t pos = controller.getPosition();    
    message = "ODM," + String(pos.x) + "," + String(pos.y) + "," + String(pos.theta);
    Serial.println(message);
	}
}

//Heartbeat Heartbeat;

