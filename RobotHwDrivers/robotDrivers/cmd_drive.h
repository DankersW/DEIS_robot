#ifndef _CMD_DRIVE_H_
#define _CMD_DRIVE_H_
#include "command.h"

class CommandDrive: public Command {
  public:
    CommandDrive(int *args);
    void execute();
  private:
    int left;
    int right;
    //void parse(String line);
};
#endif
