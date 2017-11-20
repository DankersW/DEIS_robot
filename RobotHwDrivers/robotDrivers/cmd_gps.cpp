#include "cmd_gps.h"
#include "controller.h"

CommandGPS::CommandGPS(int *args){
  x = args[0];
  y = args[1];
}


void CommandGPS::execute(){
	Serial.println("DEBUG, setting gps to: " + String(x) + ", " + String(y));
	pos_t pos = {0};
	pos.x = (double)x;
	pos.y = (double)y;
	pos.theta = theta;
	controller.updateGPS(0,0,0);
}

