#include "command.h"
#include "evaluator.h"
#include <stdlib.h>
#include "main.h"

char** evaluate() {
  Command command_type = get_command_type();

  char** output = NULL;

  switch (command_type) {
    case Echo:
      output = arguments;
      break;
    case Exit:
      if (flags_count != 0) {
        printf("%s: wrong flags count\n", command); 
        break;
      }
      if (arguments_count != 1) {
        printf("%s: wrong arguments count\n", command); 
        break;
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
        break;
      }

      int status_code = atoi(arguments[0]);

      if (status_code < 0 || status_code > 254) {
        printf("%s: status code must be between 0 and 254\n", command);
        return output;
      }

      exit(status_code);
    default:
      printf("%s: command not found\n", command);
      break;
  }

  return output;
}