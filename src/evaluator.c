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
    printf("%s ", arguments[i]);
  }

  printf("\n");
}

void evaluate_type() {
  if (arguments_count != 1) {
    printf("%s: wrong arguments count\n", command); 
    return;
  } 

  Command argument_command_type = get_command_type(arguments[0]);
  
  if (is_builtin(argument_command_type)) {
    printf("%s is a shell builtin\n", arguments[0]);
    return;
  }

  char *command_env_path = get_command_from_env_path(arguments[0]);

  if (command_env_path) {
    printf("%s is %s\n", arguments[0], command_env_path);
    free(command_env_path);
  } else printf("%s: not found\n", arguments[0]);
}

void evaluate_exit() {
  if (flags_count != 0) {
    printf("%s: no flags should be provided\n", command); 
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
    printf("%s: provide valid status code\n", command);
    return;
  }

  int status_code = atoi(arguments[0]);

  if (status_code < 0 || status_code > 254) {
    printf("%s: status code must be between 0 and 254\n", command);
    return;
  }

  exit(status_code);
}

void evaluate_pwd() {
  if (arguments_count > 0) {
    printf("%s: no arguments should be passed\n", command);
    return;
  }

  char pwd[MAX_FULL_PATH_LENGTH];   

  if (getcwd(pwd, sizeof(pwd)) == NULL) {
    perror("getcwd error detected");
    exit(1);
  } 
  
  printf("%s\n", pwd);
}

void evaluate_unknown_command(char *input) {
  char* command_env_path = get_command_from_env_path(command);
  if (!command_env_path) {
    printf("%s: command not found\n", command);
    return;
  }

  int external_program_status = system(input);

  if (external_program_status != 0) {
    perror("External program error detected\n");
    exit(1);
  }
}

void evaluate_cd() {
  if (arguments_count != 1) {
    printf("Wrong arguments count\n");
    return;
  }

  char *dir_to_cd = arguments[0];

  if (strlen(arguments[0]) == 1 && *arguments[0] == '~') {
    char *home_path = getenv("HOME");

    if (!home_path) {
      printf("HOME environment variable was not found\n");
      return;
    }

    dir_to_cd = home_path;
  }

  char success_flag = chdir(dir_to_cd);

  if (success_flag != 0) printf("%s: %s: No such file or directory\n", command, arguments[0]);
}

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
    case Pwd:
      evaluate_pwd();
      break;
    case Cd:
      evaluate_cd();
      break;
    default:
      evaluate_unknown_command(input);
      break;
  }
}