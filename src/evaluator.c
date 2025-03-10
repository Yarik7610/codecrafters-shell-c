#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "command.h"
#include "evaluator.h"
#include "main.h"
#include "builtins.h"

void evaluate() {
  Command command_type = get_command_type(command);

  switch (command_type) {
    case Echo:
      if (!arguments_count) break;

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
      break;

    case Type:
      if (flags_count != 0) {
        printf("%s: wrong flags count\n", command); 
        break;
      }
      if (arguments_count != 1) {
        printf("%s: wrong arguments count\n", command); 
        break;
      } 
      

      Command argument_command_type = get_command_type(arguments[0]);
      char* answer = is_builtin(argument_command_type) ? " is a shell builtin" : ": not found";

      output = malloc(strlen(arguments[0]) + strlen(answer) + 2);
      if (!output) {
        perror("Output element malloc failed");
        exit(1);
      }

      sprintf(output, "%s%s", arguments[0], answer);
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
        return;
      }

      exit(status_code);

    default:
      printf("%s: command not found\n", command);
      break;
  }
}