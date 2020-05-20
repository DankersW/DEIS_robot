#ifndef _COMMAND_H_
#define _COMMAND_H_

#include <Arduino.h>

class Command{
  public:
    //virtual void parse(String line);
    virtual void execute();
};
#endif
