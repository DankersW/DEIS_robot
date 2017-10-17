#include "command.h"
#include "command_factory.h"

void setup() 
{
  // put your setup code here, to run once:
  Serial.begin(9600);
  while (!Serial); // wait for serial port to connect. Needed for native USB port only

  Serial.println("SENSOR: Setup completed");
}

void loop()
{
    Serial.println("ALL,0,0");
    Command *cmd;
    while(Serial.available())
    {
      String line = Serial.readStringUntil('\n');
      cmd = CommandCreator::parse(line);
      cmd->execute();
      delete cmd;
    }
    delay(1000);
}
