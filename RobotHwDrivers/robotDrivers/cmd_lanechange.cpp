#include "cmd_lanechange.h"

CommandLaneChange::CommandLaneChange(int *args)
{
  direction = args[0];
}

void CommandLaneChange::execute(){
  if(direction == "right"){
    controller.startLaneChange(true);
  }
  else{
    controller.startLaneChange(false);
  }
}
