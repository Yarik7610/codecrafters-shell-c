#ifndef COMMAND_H
#define COMMAND_H

typedef enum {
  Exit,
  Echo,
  Type,
  Pwd,
  Unknown,
  Cd,
} Command;

Command get_command_type(char *command);

void eval_echo();
void eval_type();
void eval_exit();
void eval_pwd();
void eval_cd();
void eval_unknown_command(char *input);

#endif