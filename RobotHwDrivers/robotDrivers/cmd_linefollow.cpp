#include "cmd_linefollow.h"
#include "pin_header.h"
#include "Redbot.h"

static void driveWheels(int v[2]);
static void leftWheel(int motorPower);
static void rightWheel(int motorPower);
static void getIRvalues(int r[3]);

//RedBotSensor IRSensor0 = RedBotSensor(A3); // initialize a IRsensor object on A3 ~left
//RedBotSensor IRSensor1 = RedBotSensor(A6); // initialize a IRsensor object on A6 ~middle
//RedBotSensor IRSensor2 = RedBotSensor(A7); // initialize a IRsensor object on A7 ~right

CommandLinefollow::CommandLinefollow(int *args)
{
  speed = args[0];
}


void CommandLinefollow::execute(){
  controller.startLineFollow(speed);
}
