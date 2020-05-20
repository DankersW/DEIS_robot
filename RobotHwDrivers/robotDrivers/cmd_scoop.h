#ifndef _CMD_SCOOP_H_
#define _CMD_SCOOP_H_
#include "command.h"

class CommandScoop: public Command {
  public:
    CommandScoop(int *args);
    void execute();
  private:
    int mode;
};
#endif
