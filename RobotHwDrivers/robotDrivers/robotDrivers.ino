#include <RedBot.h>
#include "command.h"
#include "command_factory.h"
#include "pin_header.h"
#include "controller.h"

String serialInput = "";
Controller controller = Controller();
int pos_x = 0;
int pos_y = 0;
RedBotEncoder encoder = RedBotEncoder(A2, 10);  // initializes encoder on pins A2 and 10
RedBotSensor IRSensor0 = RedBotSensor(A3); // initialize a IRsensor object on A3 ~left
RedBotSensor IRSensor1 = RedBotSensor(A6); // initialize a IRsensor object on A6 ~middle
RedBotSensor IRSensor2 = RedBotSensor(A7); // initialize a IRsensor object on A7 ~right 

  
  
void setup() {
  Serial.begin(9600);
  Serial.setTimeout(0);

  pinMode(12, INPUT_PULLUP); //onboard button
  
  //left motor
  pinMode(L_CTRL1, OUTPUT);  
  pinMode(L_CTRL2, OUTPUT);  
  pinMode(L_PWM, OUTPUT); 
    
  //right motor
  pinMode(R_CTRL1, OUTPUT);  
  pinMode(R_CTRL2, OUTPUT);  
  pinMode(R_PWM, OUTPUT); 

  while (!Serial); // wait for serial port to connect. Needed for native USB port only
}

void loop() {
  //sendSensorData();
  checkReceivedCommand();
}

void checkReceivedCommand(){
  Command *cmd;
  
  int incomingB = 0;
  if(Serial.available()){
    incomingB = Serial.read();
    if(incomingB == 10){  // newline received on the channel 
      Serial.println("DEBUG,serialInput: |" + String(serialInput) + "|");
      cmd = CommandCreator::parse(serialInput);
      cmd->execute();
      Serial.println("pos actually: " + String(pos_x) + ", " + String(pos_y));
      delete cmd;
      serialInput = "";
    }
    else{
      serialInput += char(incomingB);
    }
  }
}

//void sendSensorData(){
//  int encoder[2] = {0};
//  int irValues[3] = {0};
//  getEncoderValues(encoder);
//  //getIRvalues(irValues);
//
//  String output = "All," + String(encoder[0]) + "," + String(encoder[1]) + ","  + String(irValues[0]) + "," + String(irValues[1]) + "," + String(irValues[2]) ;
//  Serial.flush();
//  Serial.println(output);
//}






