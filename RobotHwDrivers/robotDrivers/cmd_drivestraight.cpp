#include "cmd_drivestraight.h"
#include "pin_header.h"

CommandDriveStraight::CommandDriveStraight(int *args)
{
  SPEED = args[0];
  DISTANCE = args[1];
}

void CommandDriveStraight::execute(){
  Serial.println("DEBUG,Drive straight \tSPEED: " + String(SPEED) + "\t DISTANCE: " + String(DISTANCE));
}
