#ifndef _CMD_LINEFOLLOW_H_
#define _CMD_LINEFOLLOW_H_
#include "command.h"

class CommandLinefollow: public Command {
  public:
    CommandLinefollow(int *args);
    void execute();
  private:
    int speed;
};
#endif
