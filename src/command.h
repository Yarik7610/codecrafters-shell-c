typedef enum {
  Exit,
  Echo,
  Unknown
} Command;

Command get_command_type(char *command);