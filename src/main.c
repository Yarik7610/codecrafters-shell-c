#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "main.h"
#include "reader.h"
#include "trim.h"

#define EXIT_COMMAND "exit 0"

int main() {
  setbuf(stdout, NULL);
  
  char input[MAX_INPUT_SIZE];

  while (1) {
    printf("$ ");

    fgets(input, MAX_INPUT_SIZE, stdin);

    input[strlen(input) - 1] = '\0';

    char *trimmed_input = trim(input);
    if (*trimmed_input == '\0') continue;

    if (strcmp(trimmed_input, EXIT_COMMAND) == 0) exit(0);

    char *command = read_command(trimmed_input);

    printf("%s: command not found\n", trimmed_input);

    free(command);
  }

  return 0;
}
