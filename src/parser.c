#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "parser.h"
#include "main.h"
#include "utils.h"

int is_in_word_char(char ch) {
  return isalnum(ch) || ch == '_' || ch == '.' || ch == '/' || ch == '-' || ch == '~';
}

void parse_flags(char *input, int *i) {
  ++(*i);
  for (; isalnum(input[*i]) && input[*i] != '\0'; ++(*i)) {
    flags = realloc(flags, (flags_count + 1));
    if (!flags) {
      perror("Flags realloc failed");
      exit(1);
    }

    flags[flags_count++] = input[*i];
  }
}

void parse_redirect_file_path(char *input, int *i) {
  char **redirect_file_path;
  
  int is_output_descriptor = 0;

  char file_descriptor = input[*i];
  if (file_descriptor == '>' || file_descriptor == '1') {
    redirect_file_path = &stdout_file_path;
    is_output_descriptor = 1;
  }
  else if (file_descriptor == '2') redirect_file_path = &stderr_file_path;
  else {
    perror("Wrong redirect file descriptor provided\n");
    exit(1);
  }

  int offset = isdigit(file_descriptor) ? 2 : 1;
  (*i) += offset; 

  if (input[*i] == '>') {
    if (is_output_descriptor) update_redirect_file_mode(stdout_file_mode, "a");
    else update_redirect_file_mode(stderr_file_mode, "a");
    ++(*i);
  }

  while (isspace(input[*i]) && input[*i] != '\0') ++(*i);

  if (input[*i] == '\0') {
    perror("Redirect file path wasn't provided\n");
    exit(1);
  }

  int file_path_start_idx = *i;

  while (!isspace(input[*i]) && input[*i] != '\0') ++(*i);

  int file_path_len = *i - file_path_start_idx;

  *redirect_file_path = malloc(file_path_len + 1);
  if (!*redirect_file_path) {
    perror("Redirect file path malloc failed");
    exit(1);
  }

  for (int j = 0; j < file_path_len; ++j) {
    (*redirect_file_path)[j] = input[file_path_start_idx++];
  }

  (*redirect_file_path)[file_path_len] = '\0';
}

void parse_argument(char *input, int *i) {
  char quote_char;

  char temp_arg[MAX_INPUT_SIZE];
  int temp_index = 0;

  while (input[*i] != '\0') {
    if (input[*i] == '\'' || input[*i] == '\"') {
      quote_char = input[(*i)++];

      while (input[*i] != quote_char && input[*i] != '\0') {
        if (input[*i] == '\\' && quote_char == '\"') {
          char next_ch = input[*i + 1];
          if (next_ch == '\\' || next_ch == '$' || next_ch == '\"') ++(*i);
        }

        temp_arg[temp_index++] = input[(*i)++];
      }

      if (input[*i] == quote_char) ++(*i);
      else {
        printf("Wrong %c count detected\n", quote_char);
        exit(1);
      }

    } else if (is_in_word_char(input[*i])) temp_arg[temp_index++] = input[(*i)++];
    else if (input[*i] == '\\') {
      ++(*i);

      if (input[*i] == '\0') {
        perror("Wrong backslash outside of quotes");
        exit(1);
      }

      temp_arg[temp_index++] = input[(*i)++];
    }
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

void parse_input(char *input) {
  int i = 0;

  while (input[i] != '\0') {
    while (isspace(input[i]) && input[i] != '\0') ++i;

    if (input[i] == '-' && i > 0 && isspace(input[i - 1])) parse_flags(input, &i);
    else if (input[i] == '>' || (isdigit(input[i]) && input[i+1] == '>')) parse_redirect_file_path(input, &i);
    else parse_argument(input, &i);
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