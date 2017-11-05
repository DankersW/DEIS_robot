#ifndef _COMMAND_H_
#define _COMMAND_H_

#include <Arduino.h>
#include "controller.h"
#include <RedBot.h>

extern Controller controller;
//extern RedBotEncoder encoder ;  // initializes encoder on pins A2 and 10
//extern RedBotSensor IRSensor0 ; // initialize a IRsensor object on A3 ~left
//extern RedBotSensor IRSensor1 ; // initialize a IRsensor object on A6 ~middle
//extern RedBotSensor IRSensor2;
  
class Command{
  public:
    //virtual void parse(String line);
    virtual void execute();
};
#endif
