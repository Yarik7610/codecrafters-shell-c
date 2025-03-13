#include "commands.h"
#include "evaluator.h"
#include "main.h"

void evaluate(char *input) {
  Command command_type = get_command_type(arguments[0]);

  switch (command_type) {
    case Echo:
      eval_echo();
      break;
    case Type:
      eval_type();
      break;
    case Exit:
      eval_exit();
    case Pwd:
      eval_pwd();
      break;
    case Cd:
      eval_cd();
      break;
    default:
      eval_unknown_command(input);
      break;
  }
}