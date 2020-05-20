#include "cmd_drive.h"
CommandDrive::CommandDrive(int *args)
{
  left = args[0];
  right = args[1];
}

void CommandDrive::execute(){
  Serial.println("left wheel:" + String(left));
  Serial.println("right wheel:" + String(right));
}

