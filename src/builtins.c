#include <stdlib.h>
#include "builtins.h"

Command builtins[] = {Echo, Exit, Type};

int is_builtin(Command command_type) {
  size_t builtins_len = sizeof(builtins) / sizeof(builtins[0]);

  for (size_t i = 0; i < builtins_len; ++i) {
    if (builtins[i] == command_type) return 1;
  }

  return 0; 
}