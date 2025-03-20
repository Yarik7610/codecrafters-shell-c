#ifndef EXECUTABLES_H
#define EXECUTABLES_H

#include "autocomplete.h"

char* get_command_from_env_path(char *command);
void find_uncompleted_in_executables(char *uncompleted, int uncompleted_len, Matches *matches);

#endif
