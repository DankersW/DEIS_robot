// Note: The RedBot Mainboard programs as an Arduino Uno
/******************************************************************************/

// H-Bridge motor driver pins
#define    L_CTRL1   2
#define    L_CTRL2   4
#define    L_PWM     5

#define    R_CTRL1   7
#define    R_CTRL2   8
#define    R_PWM     6

// XBee SW_Serial pins
#define    SW_SER_TX A0
#define    SW_SER_RX A1


void setup()
{
    Serial.begin(9600);
    
    //left motor
    pinMode(L_CTRL1, OUTPUT);  
    pinMode(L_CTRL2, OUTPUT);  
    pinMode(L_PWM, OUTPUT); 
    
    //right motor
    pinMode(R_CTRL1, OUTPUT);  
    pinMode(R_CTRL2, OUTPUT);  
    pinMode(R_PWM, OUTPUT);  

    pinMode(13, OUTPUT);  // debug LED
}


void loop()
{
    // accelerate and stop
    digitalWrite(13, HIGH);
    int i = 0;
    for(i=30;i<=255;i=i+10){
      leftMotor(i);
      rightMotor(i);
      delay(150);
    }
    
    
    leftBrake();
    rightBrake();
    digitalWrite(13, LOW);
    delay(2000);  
}

/*******************************************************************************/
void leftMotor(int motorPower)
{
    motorPower = constrain(motorPower, -255, 255);   // constrain motorPower to -255 to +255
    if(motorPower <= 0)
    {
        // spin CCW
        digitalWrite(L_CTRL1, HIGH);
        digitalWrite(L_CTRL2, LOW);
        analogWrite(L_PWM, abs(motorPower));
    }
    else
    {
        // spin CW
        digitalWrite(L_CTRL1, LOW);
        digitalWrite(L_CTRL2, HIGH);
        analogWrite(L_PWM, abs(motorPower));
    }
}
/*******************************************************************************/
void rightMotor(int motorPower)
{
    motorPower = constrain(motorPower, -255, 255);   // constrain motorPower to -255 to +255
    if(motorPower <= 0)
    {
        // spin CCW
        digitalWrite(R_CTRL1, HIGH);
        digitalWrite(R_CTRL2, LOW);
        analogWrite(R_PWM, abs(motorPower));
    }
    else
    {
        // spin CW
        digitalWrite(R_CTRL1, LOW);
        digitalWrite(R_CTRL2, HIGH);
        analogWrite(R_PWM, abs(motorPower));
    }
}

/*******************************************************************************/
void leftBrake()
{
    // setting both controls HIGH, shorts the motor out -- causing it to self brake.
    digitalWrite(L_CTRL1, HIGH);
    digitalWrite(L_CTRL2, HIGH);
    analogWrite(L_PWM, 0);
}


/*******************************************************************************/
void rightBrake()
{
    // setting both controls HIGH, shorts the motor out -- causing it to self brake.
    digitalWrite(L_CTRL1, HIGH);
    digitalWrite(L_CTRL2, HIGH);
    analogWrite(R_PWM, 0);
}

