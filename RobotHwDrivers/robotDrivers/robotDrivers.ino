#include <RedBot.h>
#include "command.h"
#include "command_factory.h"
#include "pin_header.h"

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
  while(Serial.available()) {
    String lineInput = Serial.readString(); 
    Serial.println("DEBUG,LineInput: |" + String(lineInput) + "|");
    cmd = CommandCreator::parse(lineInput);
    cmd->execute();
    delete cmd;
    Serial.flush();
  }
}

void sendSensorData(){
  int encoder[2] = {0};
  int irValues[3] = {0};
  getEncoderValues(encoder);
  getIRvalues(irValues);

  String output = "All," + String(encoder[0]) + "," + String(encoder[1]) + ","  + String(irValues[0]) + "," + String(irValues[1]) + "," + String(irValues[2]) ;
  Serial.flush();
  Serial.println(output);
}

void getEncoderValues(int r[2]){
  // saves the encoder values into the array of the arugment r[0]=LeftWheel   r[1]=RightWheel
  r[0] = encoder.getTicks(LEFT);
  r[1] = encoder.getTicks(RIGHT);
}

void getIRvalues(int r[3]){
  // saves the IR values into the array of the arugment r[0]=LeftIR   r[1]=MiddleIR   r[2]=RightIR
  r[0] = IRSensor0.read();
  r[1] = IRSensor1.read();
  r[2] = IRSensor2.read();
}





