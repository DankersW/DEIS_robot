#include <RedBot.h>
#include "controller.h"
RedBotSensor IRSensor1 = RedBotSensor(A3); // initialize a sensor object on A3
RedBotSensor IRSensor2 = RedBotSensor(A6); // initialize a sensor object on A6
RedBotSensor IRSensor3 = RedBotSensor(A7); // initialize a sensor object on A7
RedBotEncoder encoder = RedBotEncoder(A2, 10);  // initializes encoder on pins A2 and 10

// H-Bridge motor driver pins
#define    L_CTRL1   2
#define    L_CTRL2   4
#define    L_PWM     5

#define    R_CTRL1   7
#define    R_CTRL2   8
#define    R_PWM     6

//ulta sound
#define    trigPin   A0
#define    echoPin   A1

long sensor[] = {0, 0, 0};   // Array used to store  readings for 3 sensors.
int intaWheelSpeed[2];
const int thresholeIR = 800;
int speed_max = 70;

int speed_left;
int speed_right;

int pos_x = 0;
int pos_y = 0;

boolean roadBlock=1;

Controller controller = Controller();



void setup() {
  Serial.begin(9600);   //Enable Serial Communications

  //left motor
  pinMode(L_CTRL1, OUTPUT);  
  pinMode(L_CTRL2, OUTPUT);  
  pinMode(L_PWM, OUTPUT); 
    
  //right motor
  pinMode(R_CTRL1, OUTPUT);  
  pinMode(R_CTRL2, OUTPUT);  
  pinMode(R_PWM, OUTPUT);  

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  digitalWrite(trigPin, 0);
}

void loop() {
  //roadBlock = ultraSoundCheck();
  if(roadBlock == 1){    
    laneChange();
    //speed_left = speed_max;
    //speed_right = speed_max;
    roadBlock = 0;
  }
  line_follow();
}

void line_follow(){
  sensor[0] = IRSensor1.read();
  sensor[1] = IRSensor2.read();
  sensor[2] = IRSensor3.read();

  // calculate speed from sensor readings
  if((sensor[0] < thresholeIR) && (sensor[1] > thresholeIR) && (sensor[2] < thresholeIR)){ //middle line
    intaWheelSpeed[0] = speed_max;
    intaWheelSpeed[1] = speed_max;
  }
  else if((sensor[0] < thresholeIR) && (sensor[1] > thresholeIR) && (sensor[2] > thresholeIR)){ //mid right
    intaWheelSpeed[0] = speed_max;
    intaWheelSpeed[1] = speed_max - 20;
  }
  else if((sensor[0] < thresholeIR) && (sensor[1] < thresholeIR) && (sensor[2] > thresholeIR)){ //right
    intaWheelSpeed[0] = speed_max;
    intaWheelSpeed[1] = speed_max - 40;
  }
  else if((sensor[0] > thresholeIR) && (sensor[1] > thresholeIR) && (sensor[2] < thresholeIR)){ //mid left
    intaWheelSpeed[0] = speed_max - 20;
    intaWheelSpeed[1] = speed_max;
  }
  else if((sensor[0] > thresholeIR) && (sensor[1] < thresholeIR) && (sensor[2] < thresholeIR)){ //left
    intaWheelSpeed[0] = speed_max - 40;
    intaWheelSpeed[1] = speed_max;
  }
  else{ //none of them --> keep old speed
  }

  // write speed to engines 
  driveWheels(intaWheelSpeed);
}

void laneChange(){
  input_waypoint_t inputWayPoint;
  inputWayPoint.x = 21 + 40; // change the better value
  inputWayPoint.y = 21;      // + or minus depending on the direction (21 should be the distance between two tracks  
  controller.setWaypoint(inputWayPoint);

  intaWheelSpeed[0] = -1; //value to enter the while --> todo needs to be fixed
  intaWheelSpeed[1] = -1;
  
  while((speed_left != 0) && (speed_right != 0)) { //wheels = 0 meaning controller says that the waypoint is reached 
    encoder_t vals; 
    vals.left  = encoder.getTicks(LEFT);
    vals.right = encoder.getTicks(RIGHT);
    encoder_t velocity = controller.update(vals);
    
    Serial.println("\tleftEnc: "+ String(vals.left) + "\trightEnc: " + String(vals.right) + "\tleft V: " + String(velocity.left) + "\tRight V: " + String(velocity.right));

    intaWheelSpeed[0] = velocity.left;
    intaWheelSpeed[1] = velocity.right;
    driveWheels(intaWheelSpeed); 
  }
}

boolean ultraSoundCheck(){
  long duration, distance;
  
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10); // needed for correct readings
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = (duration/2) / 29.1;

  Serial.println("Distance : " + String(distance));

  if(distance < 50){
    return 1;
  }
  else{
    return 0;
  }
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
