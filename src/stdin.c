#include <termios.h>
#include <unistd.h>
#include "stdin.h"

void enable_raw_mode(struct termios *orig_termios) {
  struct termios new_termios;

  tcgetattr(STDIN_FILENO, orig_termios);

  new_termios = *orig_termios;
  //off echo and canonical mode (pressing enter to read input)
  new_termios.c_lflag &= ~(ECHO | ICANON);
  //immediately change
  tcsetattr(STDIN_FILENO, TCSANOW, &new_termios);
}

void disable_raw_mode(struct termios *orig_termios) {
  tcsetattr(STDIN_FILENO, TCSANOW, orig_termios);
}