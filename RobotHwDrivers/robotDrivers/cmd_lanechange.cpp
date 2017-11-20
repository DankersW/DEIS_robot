#include "cmd_lanechange.h"

CommandLaneChange::CommandLaneChange(int *args)
{
  direction = args[0];
  radius_cm = args[1];
}

void CommandLaneChange::execute(){  
  if(direction == "right"){
    controller.startLaneChange(true, radius_cm);
  }
  else{
    controller.startLaneChange(false, radius_cm);
  }
}
