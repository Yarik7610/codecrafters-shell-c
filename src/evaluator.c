#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "command.h"
#include "evaluator.h"
#include "main.h"
#include "builtins.h"
#include "executables.h"

void evaluate_echo() {
  if (!arguments_count) return;

  for (int i = 0; i < arguments_count; ++i) {
    printf("%s", arguments[i]);
  }

  printf("\n");
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

  printf("%s%s\n", arguments[0], answer);
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

void evaluate_unknown_command(char *input) {
  char* command_env_path = get_command_from_env_path(command);
  if (!command_env_path) {
    printf("%s: command not found\n", command);
    return;
  }

  int external_program_status = system(input);

  if (external_program_status != 0) {
    perror("External program error detected");
    exit(1);
  }
}

// void evaluate_pwd() {
//   if (arguments_count > 0) {
//     printf("No arguments should be passed");
//     return;
//   }

//   char pwd[MAX_FULL_PATH_LENGTH];   

//   if (getcwd(pwd, sizeof(pwd)) == NULL) {
//     perror("getcwd error");
//     exit(1);
//   } else {
//     output = pwd;
//   }
// }

void evaluate(char *input) {
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
      break;
    default:
      evaluate_unknown_command(input);
      break;
  }
}