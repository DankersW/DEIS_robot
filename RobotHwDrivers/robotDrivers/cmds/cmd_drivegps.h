#ifndef _CMD_DRIVEGPS_H_
#define _CMD_DRIVEGPS_H_
#include "command.h"

class CommandDriveGps: public Command {
  public:
    CommandDriveGps(int *args);
    void execute();
  private:
    int distance;
    int posX;
    int posY;
    //void parse(String line);
};
#endif
