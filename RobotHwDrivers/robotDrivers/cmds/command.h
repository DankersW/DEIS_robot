#ifndef _COMMAND_H_
#define _COMMAND_H_

#include <Arduino.h>
#include "../controller.h"
#include <RedBot.h>

extern Controller controller;
class Command{
  public:
    virtual void execute();
};
#endif
