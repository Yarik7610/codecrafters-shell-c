#ifndef COMMAND_H
#define COMMAND_H

typedef enum {
  Exit,
  Echo,
  Type,
  Pwd,
  Unknown
} Command;

Command get_command_type(char *command);

#endif