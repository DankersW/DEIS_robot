#include "command_factory.h"
#include "cmd_stop.h"
#include "cmd_scoop.h"
#include "cmd_drive.h"

Command *CommandCreator::parse(String line){
  int i;
  int index = 0;
  int args[10];
  
  // first read the op
  i = line.indexOf(',');
  
  String type = line.substring(0,i);
  line = line.substring(i+1);
  
  type.trim();
  
  // read and parse all arguments
  while((i = line.indexOf(',')) >= 0)
  {
    String arg = line.substring(0,i);
    
    args[index++] = (arg.toInt());
    line = line.substring(i+1);
  }
  args[index++] = (line.toInt());



  // Add new Command types here
  if(type == "drive"){
    return new CommandDrive(args);
  }
  else if(type == "stop"){
    return new CommandStop(args);
  }
  else if(type == "scoop"){
    return new CommandScoop(args);
  }
  else{
    return new Command();
  }
}

