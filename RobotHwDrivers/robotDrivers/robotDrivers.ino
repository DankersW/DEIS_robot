#include <RedBot.h>
#include "command.h"
#include "command_factory.h"
#include "pin_header.h"
#include "controller.h"


RedBotEncoder encoder = RedBotEncoder(A2, 10);  // initializes encoder on pins A2 and 10
RedBotSensor IRSensor0T = RedBotSensor(A3); // initialize a IRsensor object on A3 ~left
RedBotSensor IRSensor1T = RedBotSensor(A6); // initialize a IRsensor object on A6 ~middle
RedBotSensor IRSensor2T = RedBotSensor(A7); // initialize a IRsensor object on A7 ~right 

Controller controller = Controller();

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

//  encoder_t vals;
//  line_sensors_t linevals;
//  vals.left = encoder.getTicks(LEFT);
//  vals.right = encoder.getTicks(RIGHT);
//  linevals.left = IRSensor0T.read();
//  linevals.middle = IRSensor1T.read();
//  linevals.right = IRSensor2T.read();
//  
//  controller.updateLineSensors(linevals);
//  encoder_t velocity = controller.update(vals);
//  Serial.println("\tleftEnc: "+ String(vals.left) + "\trightEnc: " + String(vals.right) + "\tIR L: " + String(linevals.left) + "\tIR M: " + String(linevals.middle) + "\tIR R: " + String(linevals.right) + "\tleft V: " + String(velocity.left) + "\tRight V: " + String(velocity.right));
//
//  int intaWheelSpeed[2] = {int(velocity.left), int(velocity.right)};
//  driveWheels(intaWheelSpeed);
}

void checkReceivedCommand(){
  Command *cmd;
  while(Serial.available()) {
    delay(50);
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
  //getIRvalues(irValues);

  String output = "All," + String(encoder[0]) + "," + String(encoder[1]) + ","  + String(irValues[0]) + "," + String(irValues[1]) + "," + String(irValues[2]) ;
  Serial.flush();
  Serial.println(output);
}

static void getEncoderValues(int r[2]){
  // saves the encoder values into the array of the arugment r[0]=LeftWheel   r[1]=RightWheel
  r[0] = encoder.getTicks(LEFT);
  r[1] = encoder.getTicks(RIGHT);
}

static void getIRvalues(int r[3]){
  // saves the IR values into the array of the arugment r[0]=LeftIR   r[1]=MiddleIR   r[2]=RightIR
  r[0] = IRSensor0T.read();
  r[1] = IRSensor1T.read();
  r[2] = IRSensor2T.read();
}

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





