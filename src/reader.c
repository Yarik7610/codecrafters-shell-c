#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "reader.h"
#include "main.h"

int is_in_word_char(char ch) {
  return isalnum(ch) || ch == '_' || ch == '.' || ch == '/' || ch == '-' || ch == '~';
}

int is_valid_char(char ch) {
  return is_in_word_char(ch) || ch == '\'' || ch == '\"';
}

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

void read_flags(char *input, int *i) {
  ++(*i);
  for (; isalnum(input[*i]) && input[*i] != '\0' && input[*i] != '\n'; ++(*i)) {
    flags = realloc(flags, (flags_count + 1));
    if (!flags) {
      perror("Flags realloc failed");
      exit(1);
    }

    flags[flags_count++] = input[*i];
  }
}

void read_argument(char *input, int *i) {
  int argument_start_idx = *i;
  char quote_char;
  int argument_len;

  char temp_arg[MAX_INPUT_SIZE];
  int temp_index = 0;

  while (input[*i] != '\0' && input[*i] != '\n') {
    if (input[*i] == '\'' || input[*i] == '\"') {
      quote_char = input[(*i)++];

      while (input[*i] != quote_char && input[*i] != '\0' && input[*i] != '\n') temp_arg[temp_index++] = input[(*i)++];

      if (input[*i] == quote_char) ++(*i);
      else {
        printf("Wrong %c count detected\n", quote_char);
        exit(1);
      }

    } else if (is_in_word_char(input[*i])) temp_arg[temp_index++] = input[(*i)++];
    else break;
  }

  temp_arg[temp_index] = '\0';

  char *argument = malloc(temp_index + 1);
  if (!argument) {
    perror("Argument malloc failed\n");
    exit(1);
  }

  strncpy(argument, temp_arg, temp_index);
  argument[temp_index] = '\0';

  arguments = realloc(arguments, (arguments_count + 1) * sizeof(char*));
  if (!arguments) {
    perror("Arguments realloc failed");
    exit(1);
  }

  arguments[arguments_count++] = argument;
}

void read_input(char *input) {
  read_command(input);
  input += strlen(command);

  int i = 0;

  while (input[i] != '\0' && input[i] != '\n') {
    while (isspace(input[i]) && input[i] != '\0' && input[i] != '\n') ++i;

    if (input[i] == '-' && i > 0 && isspace(input[i - 1])) read_flags(input, &i);
    else if (is_valid_char(input[i])) read_argument(input, &i);
    else {
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