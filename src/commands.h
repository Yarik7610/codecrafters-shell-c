#ifndef COMMANDS_H
#define COMMANDS_H

#include <stdio.h>

typedef enum {
  Exit,
  Echo,
  Type,
  Pwd,
  Unknown,
  Cd,
} Command;

Command get_command_type(char *command);

void eval_echo(FILE *output_file);
void eval_type(FILE *output_file, FILE *error_file);
void eval_exit(FILE *error_file);
void eval_pwd(FILE *output_file, FILE *error_file);
void eval_cd(FILE *error_file);
void eval_unknown_command(char *input, FILE *error_file);

#endif