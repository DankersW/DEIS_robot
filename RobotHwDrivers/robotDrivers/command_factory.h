#ifndef _CMD_FACTORY_H_
#define _CMD_FACTORY_H_

#include "command.h"

class CommandCreator{
  public:
    static Command *parse(String line);
};
#endif
