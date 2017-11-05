#include "cmd_linefollow.h"
#include "../pin_header.h"
#include "Redbot.h"

CommandLinefollow::CommandLinefollow(int *args)
{
  speed = args[0];
}


void CommandLinefollow::execute(){
  controller.startLineFollow(speed);
}