#ifndef _CMD_GPS_H_
#define _CMD_GPS_H_
#include "command.h"

class CommandGPS: public Command {
  public:
    CommandGPS(int *args);
    void execute();
  private:
    int x;
    int y;
	double theta;
};
#endif
