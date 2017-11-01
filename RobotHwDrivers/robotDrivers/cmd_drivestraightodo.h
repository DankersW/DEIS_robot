#ifndef _CMD_DRIVESTRAIGHTODO_H_
#define _CMD_DRIVESTRAIGHTODO_H_
#include "command.h"

class CommandDriveStraightOdo: public Command {
  public:
    CommandDriveStraightOdo(int *args);
    void execute();
  private:
    int SPEED;
    int DISTANCE; //in cm
    //void parse(String line);
};
#endif
