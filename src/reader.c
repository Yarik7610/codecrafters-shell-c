#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "reader.h"
#include "main.h"

void read_command(char *input) {
  int i;
  for (i = 0; !isspace(input[i]) && input[i] != '\0' && input[i] != '\n'; ++i);

  command = malloc(i + 1);
  if (!command) {
    perror("Command malloc failed\n");
    exit(1);
  }

  strncpy(command, input, i);
  command[i] = '\0';  
}

void read_input(char *input) {
  read_command(input);
  input += strlen(command);

  int i = 0;

  while (input[i] != '\0' && input[i] != '\n') {
    while (isspace(input[i]) && input[i] != '\0' && input[i] != '\n') ++i;

    if (input[i] == '-') {
      ++i;

      for (; isalnum(input[i]) && input[i] != '\0' && input[i] != '\n'; ++i) {
        flags = realloc(flags, (flags_count + 1));
        if (!flags) {
          perror("Flags realloc failed");
          exit(1);
        }

        flags[flags_count++] = input[i];
      }
    } else if (isalnum(input[i])) {
      int argument_start_idx = i;
      for (; isalnum(input[i]) && input[i] != '\0' && input[i] != '\n'; ++i);

      int argument_len = i - argument_start_idx;

      char *argument = malloc(argument_len + 1);
      if (!argument) {
        perror("Argument malloc failed\n");
        exit(1);
      }

      strncpy(argument, input + argument_start_idx, argument_len);
      argument[argument_len] = '\0';

      arguments = realloc(arguments, (arguments_count + 1) * sizeof(char*));
      if (!arguments) {
        perror("Arguments realloc failed");
        exit(1);
      }

      arguments[arguments_count++] = argument;
    } else {
      printf("Unknown char: %c\n", input[i]);
      exit(1);
    }
  }

  if (flags_count > 0) {
    flags = realloc(flags, (flags_count + 1)); 
    if (!flags) {
        perror("Flags realloc failed");
        exit(1);
    }

    flags[flags_count] = '\0'; 
  }

  if (arguments_count > 0) {
    arguments = realloc(arguments, (arguments_count + 1) * sizeof(char*)); 
    if (!arguments) {
        perror("Arguments realloc failed");
        exit(1);
    }

    arguments[arguments_count] = NULL; 
  }
}