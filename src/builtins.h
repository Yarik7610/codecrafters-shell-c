#ifndef BUILTINS_H
#define BUILTINS_H

#include "commands.h"
#include "autocomplete.h"

extern char *builtins[];

int is_builtin(char *command);
void find_uncompleted_in_builtins(char *uncompleted, int uncompleted_len, Matches *matches);

#endif