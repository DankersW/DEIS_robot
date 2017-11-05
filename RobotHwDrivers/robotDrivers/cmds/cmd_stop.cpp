#include "cmd_stop.h"
#include "../pin_header.h"
#include "../robot.h"

CommandStop::CommandStop(int *args)
{
  // nothing to do
}

void CommandStop::execute(){
  Serial.println("DEBUG,Stopping");
  
  robot.stop();
}
