#include <stdlib.h>
#include <string.h>
#include "builtins.h"

char *builtins[] = {"echo", "exit", "type", "pwd", "cd", NULL};

int is_builtin(char *command) {
  for (int i = 0; builtins[i] != NULL; ++i) {
    if (strcmp(builtins[i], command) == 0) return 1;
  }

  return 0; 
}