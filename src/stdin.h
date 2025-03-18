#ifndef STDIN_H
#define STDIN_H

#include <termios.h>
#include <unistd.h>

void enable_raw_mode(struct termios *orig_termios);
void disable_raw_mode(struct termios *orig_termios);

#endif 