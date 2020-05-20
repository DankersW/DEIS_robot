#ifndef _CMD_LANECHANGE_H_
#define _CMD_LANECHANGE_H_
#include "command.h"

class CommandLaneChange: public Command {
  public:
    CommandLaneChange(int *args);
    void execute();
  private:
    String direction;
    uint8_t radius_cm;
};
#endif
