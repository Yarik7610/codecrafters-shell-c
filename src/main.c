#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "main.h"
#include "reader.h"
#include "trim.h"
#include "evaluator.h"
#include "printer.h"
#include "command.h"

char *command = NULL;

char *flags = NULL;
int flags_count = 0;

char **arguments = NULL;
int arguments_count = 0;

char *output = NULL;

void free_globals();

int main() {
  setbuf(stdout, NULL);
  
  char input[MAX_INPUT_SIZE];

  while (1) {
    printf("$ ");

    fgets(input, MAX_INPUT_SIZE, stdin);

    input[strlen(input) - 1] = '\0';

    char *trimmed_input = trim(input);
    if (*trimmed_input == '\0') continue;
   
    read_input(trimmed_input);

    evaluate();

    print();

    free_globals();
  }

  return 0;
}


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

  free(output);
  output = NULL;
  
  flags_count = 0;
  arguments_count = 0;
}