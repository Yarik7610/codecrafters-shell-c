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

void eval_echo(FILE *output_file) {
  if (arguments_count <= 1) return;

  for (int i = 1; i < arguments_count; ++i) {
    fprintf(output_file, "%s ", arguments[i]);
  }

  fprintf(output_file, "\n");
}

void eval_type(FILE *output_file, FILE *error_file) {
  char *command = arguments[0];

  if (arguments_count != 2) {
    fprintf(error_file, "%s: wrong arguments count\n", command); 
    return;
  }

  char *first_command_argument = arguments[1];

  Command argument_command_type = get_command_type(first_command_argument);
  
  if (is_builtin(argument_command_type)) {
    fprintf(output_file, "%s is a shell builtin\n", first_command_argument);
    return;
  }

  char *command_env_path = get_command_from_env_path(first_command_argument);

  if (command_env_path) {
    fprintf(output_file, "%s is %s\n", first_command_argument, command_env_path);
    free(command_env_path);
  } else fprintf(error_file, "%s: not found\n", first_command_argument);
}

void eval_exit(FILE *error_file) {
  char *command = arguments[0];

  if (flags_count != 0) {
    fprintf(error_file, "%s: no flags should be provided\n", command); 
    return;
  }

  if (arguments_count != 2) {
    fprintf(error_file, "%s: wrong arguments count\n", command); 
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
    fprintf(error_file, "%s: provide valid status code\n", command);
    return;
  }

  int status_code = atoi(first_command_argument);

  if (status_code < 0 || status_code > 254) {
    fprintf(error_file, "%s: status code must be between 0 and 254\n", command);
    return;
  }

  exit(status_code);
}

void eval_pwd(FILE *output_file, FILE *error_file) {
  if (arguments_count > 1) {
    fprintf(error_file, "%s: no arguments should be passed\n", arguments[0]);
    return;
  }

  char pwd[MAX_FULL_PATH_LENGTH];   

  if (getcwd(pwd, sizeof(pwd)) == NULL) {
    perror("getcwd error detected");
    exit(1);
  } 
  
  fprintf(output_file, "%s\n", pwd);
}

void eval_cd(FILE *error_file) {
  char *command = arguments[0];

  if (arguments_count != 2) {
    fprintf(error_file, "%s: wrong arguments count\n", command);
    return;
  }

  char *first_command_argument = arguments[1];

  char *dir_to_cd = first_command_argument;

  if (strlen(first_command_argument) == 1 && *first_command_argument == '~') {
    char *home_path = getenv("HOME");

    if (!home_path) {
      fprintf(error_file, "HOME environment variable was not found\n");
      return;
    }

    dir_to_cd = home_path;
  }

  char success_flag = chdir(dir_to_cd);
  if (success_flag != 0) fprintf(error_file, "%s: %s: No such file or directory\n", command, first_command_argument);
}

void eval_unknown_command(char *input, FILE *error_file) {
  char *command = arguments[0];
  
  char* command_env_path = get_command_from_env_path(command);
  if (!command_env_path) {
    fprintf(error_file, "%s: command not found\n", command);
    return;
  }

  system(input);
}