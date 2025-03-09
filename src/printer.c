#include "printer.h"
#include "command.h"
#include <stdio.h>

void print(char *command, char *flags, char *args[]) {
  Command command_type = get_command_type(command);

  switch (command_type) {
    case Echo:
      break;
    case Exit:
      exit(0);
      break;
    default:
      printf("%s: command not found\n", command);
      break;
  }
}