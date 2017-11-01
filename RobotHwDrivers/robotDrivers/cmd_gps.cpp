#include "cmd_gps.h"
extern int pos_x;
extern int pos_y;
CommandGPS::CommandGPS(int *args){
  x = args[0];
  y = args[1];
}


void CommandGPS::execute(){
  Serial.println("DEBUG, setting gps to: " + String(x) + ", " + String(y));
  pos_x = x;
  pos_y = y;
}

