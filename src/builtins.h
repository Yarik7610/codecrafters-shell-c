#ifndef BUILTINS_H
#define BUILTINS_H

#include "commands.h"

extern Command builtins[];

int is_builtin(Command command_type);

#endif