#ifndef BUILTINS_H
#define BUILTINS_H

#include "commands.h"

extern char *builtins[];

int is_builtin(char *command);

#endif