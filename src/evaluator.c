#include "commands.h"
#include "evaluator.h"
#include "main.h"
#include "utils.h"

void evaluate(char *input) {
  Command command_type = get_command_type(arguments[0]);

  FILE *output_file = get_redirect_file(&redirect_out_info);
  if (!output_file) output_file = stdout;
  FILE *error_file = get_redirect_file(&redirect_err_info);
  if (!error_file) error_file = stderr;

  switch (command_type) {
    case Echo:
      eval_echo(output_file);
      break;
    case Type:
      eval_type(output_file, error_file);
      break;
    case Exit:
      eval_exit(error_file);
      break;
    case Pwd:
      eval_pwd(output_file, error_file);
      break;
    case Cd:
      eval_cd(error_file);
      break;
    default:
      eval_unknown_command(input, error_file);
      break;
  }

  close_redirect_file(output_file);
  close_redirect_file(error_file);
}