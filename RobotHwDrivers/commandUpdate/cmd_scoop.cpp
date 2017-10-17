#include "cmd_scoop.h"
CommandScoop::CommandScoop(int *args)
{
  mode = args[0];
}

void CommandScoop::execute(){
  Serial.println("Scoop mode:" + String(mode));
}

