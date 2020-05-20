#include <RedBot.h>
RedBotSensor IRSensor1 = RedBotSensor(A3); // initialize a sensor object on A3
RedBotSensor IRSensor2 = RedBotSensor(A6); // initialize a sensor object on A6
RedBotSensor IRSensor3 = RedBotSensor(A7); // initialize a sensor object on A7

// H-Bridge motor driver pins
#define    L_CTRL1   2
#define    L_CTRL2   4
#define    L_PWM     5

#define    R_CTRL1   7
#define    R_CTRL2   8
#define    R_PWM     6

long sensor[] = {0, 0, 0};   // Array used to store  readings for 3 sensors.

const int thresholeIR = 800;
int speed_max = 70;

int error;
int speed_left;
int speed_right;

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

}

void loop() {
  // put your main code here, to run repeatedly:
  line_follow();
}

void line_follow(){

  
  sensor[0] = IRSensor1.read();
  sensor[1] = IRSensor2.read();
  sensor[2] = IRSensor3.read();
  
  if((sensor[0] < thresholeIR) && (sensor[1] > thresholeIR) && (sensor[2] < thresholeIR)){
    Serial.println("Middle line");
    speed_left = speed_max;
    speed_right = speed_max;
  }
  else if((sensor[0] < thresholeIR) && (sensor[1] > thresholeIR) && (sensor[2] > thresholeIR)){
    Serial.println("mid right");
    speed_left = speed_max;
    speed_right = speed_max - 20;
  }
  else if((sensor[0] < thresholeIR) && (sensor[1] < thresholeIR) && (sensor[2] > thresholeIR)){
    Serial.println("right");
    speed_left = speed_max;
    speed_right = speed_max - 40;
  }
  else if((sensor[0] > thresholeIR) && (sensor[1] > thresholeIR) && (sensor[2] < thresholeIR)){
    Serial.println("mid left");
    speed_left = speed_max - 20;
    speed_right = speed_max;
  }
  else if((sensor[0] > thresholeIR) && (sensor[1] < thresholeIR) && (sensor[2] < thresholeIR)){
    Serial.println("left");
    speed_left = speed_max - 40;
    speed_right = speed_max;
  }
  else{
    Serial.println("no line");
  }

  //open the Serial Monitor to see the speed of each motor
  Serial.print ("right = ");
  Serial.print(speed_right); 
  Serial.print("\t");
  Serial.print("left = "); 
  Serial.println(speed_left); 

  //Serial.println("DEBUG,Wheel speed\tLeft: " + String(left) + "\t Right: " + String(right));
  int intaWheelSpeed[2] = {speed_left, speed_right};
  driveWheels(intaWheelSpeed);
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
