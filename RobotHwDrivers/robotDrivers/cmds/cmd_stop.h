#ifndef _CMD_STOP_H_
#define _CMD_STOP_H_
#include "command.h"

class CommandStop: public Command {
  public:
    CommandStop(int *args);
    void execute();
  private:
};
#endif

