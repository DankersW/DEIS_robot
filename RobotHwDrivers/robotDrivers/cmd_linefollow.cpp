#include "cmd_linefollow.h"

CommandLinefollow::CommandLinefollow(int *args)
{
  speed = args[0];
}

void CommandLinefollow::execute(){
  controller.startLineFollow(speed);
}
