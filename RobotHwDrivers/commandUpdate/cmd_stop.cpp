#include "cmd_stop.h"
CommandStop::CommandStop(int *args)
{
  // nothing to do
}

void CommandStop::execute(){
  Serial.println("Stopping:");
}

