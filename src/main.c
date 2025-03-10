#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "main.h"
#include "reader.h"
#include "trim.h"
#include "evaluator.h"
#include "printer.h"

char *command = NULL;
char *flags = NULL;
int flags_count = 0;
char **arguments = NULL;
int arguments_count = 0;

void free_globals() {
  free(command);
  command = NULL;

  free(flags);
  flags = NULL;

  for (int i = 0; i < arguments_count; ++i) {
    free(arguments[i]);
    arguments[i] = NULL;
  } 

  free(arguments);
  arguments = NULL;
  
  flags_count = 0;
  arguments_count = 0;
}


int main() {
  setbuf(stdout, NULL);
  
  char input[MAX_INPUT_SIZE];

  while (1) {
    printf("$ ");

    fgets(input, MAX_INPUT_SIZE, stdin);

    input[strlen(input) - 1] = '\0';

    char *trimmed_input = trim(input);
    if (*trimmed_input == '\0') continue;

    int is_error = read_input(trimmed_input);
    if (is_error) exit(1);

    // printf("DEBUG, command: %s\n", command);
    // printf("DEBUG, flags: %s\n", flags);
    // for (int i = 0; i < arguments_count; ++i) {
    //   printf("DEBUG, arguments[%d]: %s\n", i, arguments[i]);
    // }

    char **output = evaluate();
    
    print(output);

    free_globals();
  }

  return 0;
}
