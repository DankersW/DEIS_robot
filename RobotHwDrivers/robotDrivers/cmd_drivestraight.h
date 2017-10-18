#ifndef _CMD_DRIVESTRAIGHT_H_
#define _CMD_DRIVESTRAIGHT_H_
#include "command.h"

class CommandDriveStraight: public Command {
  public:
    CommandDriveStraight(int *args);
    void execute();
  private:
    int SPEED;
    int DISTANCE; //in cm
    //void parse(String line);
};
#endif
