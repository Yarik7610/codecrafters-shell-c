#ifndef BUILTINS_H
#define BUILTINS_H

#include "commands.h"

extern char *builtins[];

int is_builtin(char *command);
char* find_uncompleted_in_builtins(char *word_buffer, int word_len);

#endif