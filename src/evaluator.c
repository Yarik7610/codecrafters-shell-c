#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "command.h"
#include "evaluator.h"
#include "main.h"
#include "builtins.h"
#include "executables.h"

void evaluate_echo() {
  if (!arguments_count) return;

  size_t total_output_len = 0;

  for (int i = 0; i < arguments_count; ++i) {
    total_output_len += strlen(arguments[i]);
  }
  total_output_len += (arguments_count - 1);

  output = malloc(total_output_len + 1);
  if (!output) {
    perror("Output malloc failed");
    exit(1);
  }

  output[0] = '\0'; 
  for (int i = 0; i < arguments_count; i++) {
    strcat(output, arguments[i]); 
    if (i < arguments_count - 1) strcat(output, " "); 
  }
}

void evaluate_type() {
  if (flags_count != 0) {
    printf("%s: wrong flags count\n", command); 
    return;
  }
  if (arguments_count != 1) {
    printf("%s: wrong arguments count\n", command); 
    return;
  } 

  Command argument_command_type = get_command_type(arguments[0]);
  
  char* answer;
  int should_free_answer = 0;

  if (is_builtin(argument_command_type)) answer = " is a shell builtin";
  else {
    char* command_env_path = get_command_from_env_path(arguments[0]);
    if (command_env_path) {
      answer = malloc(strlen(" is ") + strlen(command_env_path) + 1);
      if (!answer) {
        perror("Type: answer malloc failed");
        exit(1);
      }

      sprintf(answer, " is %s", command_env_path);

      free(command_env_path);
      should_free_answer = 1;
    }
    else answer = ": not found";
  }

  output = malloc(strlen(arguments[0]) + strlen(answer) + 1);
  if (!output) {
    perror("Output element malloc failed");
    exit(1);
  }

  sprintf(output, "%s%s", arguments[0], answer);

  if (should_free_answer) free(answer);
}

void evaluate_exit() {
  if (flags_count != 0) {
    printf("%s: wrong flags count\n", command); 
    return;
  }
  if (arguments_count != 1) {
    printf("%s: wrong arguments count\n", command); 
    return;
  } 

  int is_arg_a_number = 1;
  for (int i = 0; arguments[0][i] != '\0'; ++i) {
    if (!isdigit(arguments[0][i])) {
      is_arg_a_number = 0;
      break;
    }
  }
  if (!is_arg_a_number) {
    printf("%s: provide status code\n", command);
    return;
  }

  int status_code = atoi(arguments[0]);

  if (status_code < 0 || status_code > 254) {
    printf("%s: status code must be between 0 and 254\n", command);
    return;
  }

  exit(status_code);
}

void evaluate() {
  Command command_type = get_command_type(command);

  switch (command_type) {
    case Echo:
      evaluate_echo();
      break;
    case Type:
      evaluate_type();
      break;
    case Exit:
      evaluate_exit();
    default:
      printf("%s: command not found\n", command);
      break;
  }
}