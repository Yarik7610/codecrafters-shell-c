#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "commands.h"
#include "main.h"
#include "executables.h"
#include "builtins.h"

Command get_command_type(char *command) {
  if (strcmp(command, "echo") == 0) return Echo;
  if (strcmp(command, "exit") == 0) return Exit;
  if (strcmp(command, "type") == 0) return Type;
  if (strcmp(command, "pwd") == 0) return Pwd;
  if (strcmp(command, "cd") == 0) return Cd;
  return Unknown;
}

void eval_echo() {
  if (arguments_count <= 1) return;

  for (int i = 1; i < arguments_count; ++i) {
    printf("%s ", arguments[i]);
  }

  printf("\n");
}

void eval_type() {
  char *command = arguments[0];
  char *first_command_argument = arguments[1];

  if (arguments_count != 2) {
    printf("%s: wrong arguments count\n", command); 
    return;
  } 

  Command argument_command_type = get_command_type(first_command_argument);
  
  if (is_builtin(argument_command_type)) {
    printf("%s is a shell builtin\n", first_command_argument);
    return;
  }

  char *command_env_path = get_command_from_env_path(first_command_argument);

  if (command_env_path) {
    printf("%s is %s\n", first_command_argument, command_env_path);
    free(command_env_path);
  } else printf("%s: not found\n", first_command_argument);
}

void eval_exit() {
  char *command = arguments[0];

  if (flags_count != 0) {
    printf("%s: no flags should be provided\n", command); 
    return;
  }

  if (arguments_count != 2) {
    printf("%s: wrong arguments count\n", command); 
    return;
  } 

  int is_arg_a_number = 1;
  char *first_command_argument = arguments[1];

  for (int i = 0; first_command_argument[i] != '\0'; ++i) {
    if (!isdigit(first_command_argument[i])) {
      is_arg_a_number = 0;
      break;
    }
  }

  if (!is_arg_a_number) {
    printf("%s: provide valid status code\n", command);
    return;
  }

  int status_code = atoi(command);

  if (status_code < 0 || status_code > 254) {
    printf("%s: status code must be between 0 and 254\n", command);
    return;
  }

  exit(status_code);
}

void eval_pwd() {
  if (arguments_count > 1) {
    printf("%s: no arguments should be passed\n", arguments[0]);
    return;
  }

  char pwd[MAX_FULL_PATH_LENGTH];   

  if (getcwd(pwd, sizeof(pwd)) == NULL) {
    perror("getcwd error detected");
    exit(1);
  } 
  
  printf("%s\n", pwd);
}

void eval_cd() {
  char *command = arguments[0];
  char *first_command_argument = arguments[1];

  if (arguments_count != 2) {
    printf("%s: wrong arguments count\n", command);
    return;
  }

  char *dir_to_cd = first_command_argument;

  if (strlen(first_command_argument) == 1 && *first_command_argument == '~') {
    char *home_path = getenv("HOME");

    if (!home_path) {
      printf("HOME environment variable was not found\n");
      return;
    }

    dir_to_cd = home_path;
  }

  char success_flag = chdir(dir_to_cd);

  if (success_flag != 0) printf("%s: %s: No such file or directory\n", command, first_command_argument);
}

void eval_unknown_command(char *input) {
  char *command = arguments[0];

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