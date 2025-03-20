#include <stdlib.h>
#include <string.h>
#include "builtins.h"
#include "autocomplete.h"

char *builtins[] = {"echo", "exit", "type", "pwd", "cd", NULL};

int is_builtin(char *command) {
  for (int i = 0; builtins[i] != NULL; ++i) {
    if (strcmp(builtins[i], command) == 0) return 1;
  }

  return 0; 
}

void find_uncompleted_in_builtins(char *uncompleted, int uncompleted_len, Matches *matches) {
  for (int i = 0; builtins[i] != NULL; ++i) {
    if (strncmp(builtins[i], uncompleted, uncompleted_len) == 0) {
      matches->words = realloc(matches->words, (matches->count + 2) * sizeof(char *));
      if (!matches->words) {
        perror("Builtins matches realloc failed");
        exit(1);
      }

      char *match_copy = strdup(builtins[i]);
      if (!match_copy) {
        perror("Builtins match copy malloc failed");
        exit(1);
      }

      matches->words[matches->count++] = match_copy;
      matches->words[matches->count] = NULL;
    }
  }
}

