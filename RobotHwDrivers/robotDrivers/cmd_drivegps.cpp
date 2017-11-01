#include "cmd_drivegps.h"
#include "pin_header.h"

static void driveWheels(int v[2]);
static void leftWheel(int motorPower);
static void rightWheel(int motorPower);
String cmd;
int args[10];
CommandDriveGps::CommandDriveGps(int *args){
  distance = args[0];
  posX = args[1];
  posY = args[2];
}

bool readSerial(){
  static String line = "";
  if(Serial.available()){
      char incomingB = Serial.read();
      if(incomingB == 10){  // newline received on the channel 
        Serial.println("DEBUG,command: |" + String(line) + "|");
        int i;
        int index = 0;
        
        // first read the op
        i = line.indexOf(',');
        
        cmd = line.substring(0,i);
        line = line.substring(i+1);
        
        cmd.trim();
        
        // read and parse all arguments
        while((i = line.indexOf(',')) >= 0)
        {
          String arg = line.substring(0,i);
          
          args[index++] = (arg.toInt());
          line = line.substring(i+1);
        }
        args[index++] = (line.toInt());
        // get current X-pos and Y-pos
        //Serial.println("Xpos: " + String(posX));
        
        line = "";
        return true;
      }
      else{
        line += char(incomingB);
        
        return false;

        
        
      }
  }
}

void CommandDriveGps::execute(){
  boolean gpsDrive = true;
  int posXcurrent = 0;
  int posYcurrent = 0;
  String command = "";

  Serial.println("Xpos: " + String(posX));
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
}
#if 0  
  while(gpsDrive){    
    int incomingB = 0;
    if(Serial.available()){
      incomingB = Serial.read();
      if(incomingB == 10){  // newline received on the channel 
        Serial.println("DEBUG,command: |" + String(command) + "|");

        // get current X-pos and Y-pos
        Serial.println("Xpos: " + String(posX));

        

        
        int intaWheelSpeed[2] = {100, 100};
        driveWheels(intaWheelSpeed);

        if(command == "stop"){ //and distance has reached
          gpsDrive = false;
          int intaWheelSpeed[2] = {0, 0};
          driveWheels(intaWheelSpeed);
        }
        
        command = "";
      }
      else{
        command += char(incomingB);
        int i;
        int index = 0;
        int args[10];
        
        // first read the op
        i = line.indexOf(',');
        
        String type = line.substring(0,i);
        line = line.substring(i+1);
        
        type.trim();
        
        // read and parse all arguments
        while((i = line.indexOf(',')) >= 0)
        {
          String arg = line.substring(0,i);
          
          args[index++] = (arg.toInt());
          line = line.substring(i+1);
        }
        args[index++] = (line.toInt());

        float dx = args[1]-PosX;
        float dy = args[2]-PosY;

        r = sqrt(pow(dx,2) + pow(dy,2));

        
        
      } 
    }
      
  } 
}
#endif
static void driveWheels(int v[2]){
  // v[0] = leftWheel     v[1] = rightWheel
  leftWheel(v[0]);
  rightWheel(v[1]);
}

static void leftWheel(int motorPower){
    motorPower = constrain(motorPower, -255, 255);   // constrain motorPower to -255 to +255
    if(motorPower <= 0){ // spin CCW
        digitalWrite(L_CTRL1, HIGH);
        digitalWrite(L_CTRL2, LOW);
        analogWrite(L_PWM, abs(motorPower));
    }else{ // spin CW
        digitalWrite(L_CTRL1, LOW);
        digitalWrite(L_CTRL2, HIGH);
        analogWrite(L_PWM, abs(motorPower));
    }
}

static void rightWheel(int motorPower){
    motorPower = constrain(motorPower, -255, 255);   // constrain motorPower to -255 to +255
    if(motorPower <= 0){  // spin CCW
        digitalWrite(R_CTRL1, HIGH);
        digitalWrite(R_CTRL2, LOW);
        analogWrite(R_PWM, abs(motorPower));
    }else{ // spin CW
        digitalWrite(R_CTRL1, LOW);
        digitalWrite(R_CTRL2, HIGH);
        analogWrite(R_PWM, abs(motorPower));
    }
}
