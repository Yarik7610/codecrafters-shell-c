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

char* find_uncompleted_in_builtins(char *uncompleted, int uncompleted_len) {
  char *match = NULL;

  for (int i = 0; builtins[i] != NULL; ++i) {
    if (strncmp(uncompleted, builtins[i], uncompleted_len) == 0) {
      match = builtins[i];
      return match;
    }
  }

  return match;
}

