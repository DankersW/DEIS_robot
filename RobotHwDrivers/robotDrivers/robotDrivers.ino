#include <RedBot.h>
#include <Servo.h>

typedef struct drive_cmd
{
  int left;
  int right;
  int padding[8];
}drive_cmd_t;

typedef struct scoop_cmd
{
  int orientation;
  int padding[9];
}scoop_cmd_t;

typedef struct cmd
{
  String type;
  union{
    int args[10];
    drive_cmd_t drive_cmd;
    scoop_cmd_t scoop_cmd;
  };
  int numberOfArgs;
}cmd_t;

RedBotEncoder encoder = RedBotEncoder(A2, 10);  // initializes encoder on pins A2 and 10

// H-Bridge motor driver pins
#define    L_CTRL1   2
#define    L_CTRL2   4
#define    L_PWM     5

#define    R_CTRL1   7
#define    R_CTRL2   8
#define    R_PWM     6

RedBotSensor IRSensor0 = RedBotSensor(A3); // initialize a IRsensor object on A3 ~left
RedBotSensor IRSensor1 = RedBotSensor(A6); // initialize a IRsensor object on A6 ~middle
RedBotSensor IRSensor2 = RedBotSensor(A7); // initialize a IRsensor object on A7 ~right

Servo servoRight;  // create servo object to control the Rightservo
Servo servoLeft;  // create servo object to control the Leftservo 

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

  servoRight.attach(9);  // attaches the servo on pin 9 to the servo object
  servoLeft.attach(3);   // attaches the servo on pin 3 to the servo object
  servoRight.write(180);  //bring servo's to a
  servoLeft.write(0);     //safe start postion
  
  delay(1000);

  while (!Serial); // wait for serial port to connect. Needed for native USB port only
}

void loop() {
  sendSensorData();
  checkReceivedCommand();
}

void checkReceivedCommand(){
  cmd_t command = {};
  String lineInput = "";
  while(Serial.available()) {
    
    lineInput = Serial.readString();// read the incoming data as string
    Serial.println(lineInput);
    command = parseCommand(lineInput);
    
    if(command.type == "drive"){
      int intaWheelSpeed[2] = {command.drive_cmd.left, command.drive_cmd.right};
      driveWheels(intaWheelSpeed);
    }
    else if(command.type == "stop"){
      stopWheels();
    }
    else if(command.type == "scoop"){
      angleScoop(command.scoop_cmd.orientation);
    }
  }
}

void sendSensorData(){
  // Send wheel encoder values over the channel
  int encoder[2] = {0};
  getEncoderValues(encoder);
  String str = String(String(encoder[0]) + "," + String(encoder[1]));
  Serial.print("ALL,");
  Serial.print(str);

  // Send IR values over the channel
  int irValues[3] = {0};
  getIRvalues(irValues);
  str = String(","  + String(irValues[0]) + "," + String(irValues[1]) + "," + String(irValues[2]));
  Serial.println(str);
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

void driveWheels(int v[2]){
  // v[0] = leftWheel     v[1] = rightWheel
  leftWheel(v[0]);
  rightWheel(v[1]);
}

void stopWheels(){
  leftWheelBreak();
  rightWheelBreak();
}

void leftWheel(int motorPower){
    motorPower = constrain(motorPower, -255, 255);   // constrain motorPower to -255 to +255
    if(motorPower <= 0){ // spin CCW
        digitalWrite(L_CTRL1, HIGH);
        digitalWrite(L_CTRL2, LOW);
        analogWrite(L_PWM, abs(motorPower));
    }
    else{ // spin CW
        digitalWrite(L_CTRL1, LOW);
        digitalWrite(L_CTRL2, HIGH);
        analogWrite(L_PWM, abs(motorPower));
    }
}

void rightWheel(int motorPower){
    motorPower = constrain(motorPower, -255, 255);   // constrain motorPower to -255 to +255
    if(motorPower <= 0){  // spin CCW
        digitalWrite(R_CTRL1, HIGH);
        digitalWrite(R_CTRL2, LOW);
        analogWrite(R_PWM, abs(motorPower));
    }
    else{ // spin CW
        digitalWrite(R_CTRL1, LOW);
        digitalWrite(R_CTRL2, HIGH);
        analogWrite(R_PWM, abs(motorPower));
    }
}

void leftWheelBreak(){
    // setting both controls HIGH, shorts the motor out -- causing it to self brake.
    digitalWrite(L_CTRL1, HIGH);
    digitalWrite(L_CTRL2, HIGH);
    analogWrite(L_PWM, 0);
}

void rightWheelBreak(){  
    // setting both controls HIGH, shorts the motor out -- causing it to self brake.
    digitalWrite(L_CTRL1, HIGH);
    digitalWrite(L_CTRL2, HIGH);
    analogWrite(R_PWM, 0);
}

void angleScoop(int postionOtherRobots){
  // 0 = left   1 = right  2 = both 
  // 180° closes    30° open
  switch (postionOtherRobots){
    case 0: // other robot is on the left side
      servoRight.write(30);  
      servoLeft.write(0);
      break;
    case 1: // other robot is on the right side
      servoRight.write(180);  
      servoLeft.write(150);
      break;
    case 2: // robots on both sides
      servoRight.write(180);  
      servoLeft.write(0);
      break;
    default:
      break;
  }
}

cmd_t parseCommand(String line)
{
      int i;
      int index = 0;
      cmd_t tmp = {};
      // first read the op
      i = line.indexOf(',');
      tmp.type = line.substring(0,i);
      line = line.substring(i+1);
      
      // read and parse all arguments
      while((i = line.indexOf(',')) >= 0)
      {
        String arg = line.substring(0,i);
        
        tmp.args[index++] = arg.toInt();
        line = line.substring(i+1);
      }
      tmp.args[index++] = line.toInt();
      tmp.numberOfArgs = index;
      index = 0;
      return tmp;
}



