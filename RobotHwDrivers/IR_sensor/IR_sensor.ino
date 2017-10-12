#include <RedBot.h>
RedBotSensor IRSensor1 = RedBotSensor(A3); // initialize a sensor object on A3
RedBotSensor IRSensor2 = RedBotSensor(A6); // initialize a sensor object on A6
RedBotSensor IRSensor3 = RedBotSensor(A7); // initialize a sensor object on A7

void setup()
{
  Serial.begin(9600);
}

void loop()
{
  Serial.print("IR Sensor Readings: ");
  Serial.print(IRSensor1.read()); 
  Serial.print(" ");  // tab character
  Serial.print(IRSensor2.read());
  Serial.print(" ");  // tab character
  Serial.print(IRSensor3.read()); 
  Serial.println();
  delay(100);
}
