#include <string.h>
#include "commands.h"
#include "evaluator.h"
#include "main.h"
#include "utils.h"

void evaluate(char *input) {
  char *command = arguments[0];

  FILE *output_file = get_redirect_file(&redirect_out_info);
  if (!output_file) output_file = stdout;
  FILE *error_file = get_redirect_file(&redirect_err_info);
  if (!error_file) error_file = stderr;

  if (strcmp(command, "echo") == 0) eval_echo(output_file);
  else if (strcmp(command, "exit") == 0) eval_exit(error_file);
  else if (strcmp(command, "cd") == 0) eval_cd(error_file);
  else if (strcmp(command, "type") == 0) eval_type(output_file, error_file);
  else if (strcmp(command, "pwd") == 0) eval_pwd(output_file, error_file);
  else eval_unknown_command(input, error_file);

  close_redirect_file(output_file);
  close_redirect_file(error_file);
}