//////////////////////////////////////////////////////////////////////////
// DEPRECATED. NOW ONLY USES ONE DRIVE STRAIGHT.
//////////////////////////////////////////////////////////////////////////

#include "cmd_drivestraightodo.h"
#include "controller.h"
#include <RedBot.h>
#include "cmd_stop.h"

static void driveWheels(int v[2]);
static void leftWheel(int motorPower);
static void rightWheel(int motorPower);

CommandDriveStraightOdo::CommandDriveStraightOdo(int *args)
{
  SPEED = args[0];
  DISTANCE = args[1];
}

void CommandDriveStraightOdo::execute(){
  //(pos_t start = {0}, encoder_t encoders = {0}
  Serial.println("DEBUG,Drive straight ODO: \tSPEED: " + String(SPEED) + "\t DISTANCE: " + String(DISTANCE));

  cmd_input_t inputVal;
  inputVal.distance = DISTANCE;
  inputVal.speedOg = SPEED;
  
  //-controller.setWaypoint(inputVal);
  Serial.println("DISTANCE INIT :" + String(inputVal.distance));
}
