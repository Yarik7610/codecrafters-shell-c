#include "command.h"
#include <string.h>

Command get_command_type(char *command) {
    if (strcmp(command, "echo") == 0) {
        return Echo;
    } else if (strcmp(command, "exit") == 0) {
        return Exit;
    } else {
        return Unknown;
    }
}