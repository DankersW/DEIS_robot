#include "cmd_drivegps.h"

CommandDriveGps::CommandDriveGps(int *args){
  distance = args[0];
  posX = args[1];
  posY = args[2];
}


void CommandDriveGps::execute(){
  boolean gpsDrive = true;
  int posXcurrent = 0;
  int posYcurrent = 0;
  String command = "";

  Serial.println("Xpos: " + String(posX));
#if 0
  int intaWheelSpeed[2] = {100, 100};
  driveWheels(intaWheelSpeed);
  cmd = "";
  while(gpsDrive){
    if(readSerial()){
      
        if(cmd == "gps"){
          float dx = args[0]-posX;
          float dy = args[1]-posY;

          float r = sqrt(pow(dx,2) + pow(dy,2));
          if(r > distance){
            cmd = "stop";
          }
          
        }
        
        if(cmd == "stop"){ //and distance has reached
          gpsDrive = false;
          int intaWheelSpeed[2] = {0, 0};
          driveWheels(intaWheelSpeed);
        }
    }
  }
#endif
}
