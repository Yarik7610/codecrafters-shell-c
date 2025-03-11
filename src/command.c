#include "command.h"
#include <string.h>
#include "main.h"

Command get_command_type(char *command) {
  if (strcmp(command, "echo") == 0) return Echo;
  if (strcmp(command, "exit") == 0) return Exit;
  if (strcmp(command, "type") == 0) return Type;
  if (strcmp(command, "pwd") == 0) return Pwd;
  return Unknown;
}