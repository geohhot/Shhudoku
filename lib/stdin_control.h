
#include <termios.h>
#include <stdio.h>

struct termios tp;
struct termios old;

void store_old (void) {
  tcgetattr (fileno (stdin), &old);
  tcgetattr (fileno (stdin), &tp);
}

void set_raw_mode (void) {
  tp.c_lflag &= ~ (ECHO | ICANON);
  tcsetattr (fileno (stdin),			\
	     TCSANOW,				\
	     &tp);
}

void set_old_settings (void) {
  tcsetattr (fileno (stdin),			\
	     TCSANOW,				\
	     &old);  
}
