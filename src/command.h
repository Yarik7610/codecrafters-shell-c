#ifndef COMMAND_H
#define COMMAND_H

typedef enum {
  Exit,
  Echo,
  Unknown
} Command;

Command get_command_type();

#endif