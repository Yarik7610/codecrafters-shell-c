#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "reader.h"
#include "main.h"

char* read_command(char *input) {
  char *command = NULL;
 
  if (strcmp(input, "exit 0") == 0) exit(0);

  int i;
  for (i = 0; !isspace(input[i]) && input[i] != '\0' && input[i] != '\n'; ++i);

  command = malloc(i + 1);
  if (!command) {
    perror("Command malloc failed");
    exit(1);
  }

  strncpy(command, input, i);
  command[i] = '\0';  

  return command;
}