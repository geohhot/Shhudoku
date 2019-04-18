#include <unistd.h>
#include <stdio.h>
#include <signal.h>
#include <termios.h>
#include <stdlib.h>
#include <pthread.h>

#include "easy_tcc.h"
#include "stdin_control.h"
#include "signalling.h"
// include pthreads

pthread_t pt;
struct shared_data {
  int keycode;
  int x, y;
  int *field;
} sd;
// runs as a pthread
void* handle_user_input (void* p) {
  struct shared_data *sd = (struct shared_data *)p;
  while (1) {
    char c;
    scanf ("%c", &c);
    switch (c) {
    case 'q':
      sd->x = 1;
      sd->y = 1;
      break;
    case 'w':
      sd->x = 1;
      sd->y = 4;
      break;
    case 'e':
      sd->x = 1;
      sd->y = 7;
      break;
    case 'a':
      sd->x = 4;
      sd->y = 1;
      break;
    case 's':
      sd->x = 4;
      sd->y = 4;
      break;
    case 'd':
      sd->x = 4;
      sd->y = 7;
      break;
    case 'z':
      sd->x = 7;
      sd->y = 1;
      break;
    case 'x':
      sd->x = 7;
      sd->y = 4;
      break;
    case 'c':
      sd->x = 7;
      sd->y = 7;
      break;
    default:
      break;
    }
    if (c >= '0' && c <= '9') {
      *(sd->field + sd->x*9 + sd->y) = c - '0';
    }
    // printf ("%d ", (int)c);
    if (sd->keycode == 27 && c == 91) {
      scanf ("%c", &c);
      switch (c) {
      case 68:
	sd->y --;
	if (sd->y == -1) sd->y = 8;
	//puts ("Left");
	break;
      case 65:
	sd->x --;
	if (sd->x == -1) sd->x = 8;
	//puts ("Up");
	break;
      case 67:
	sd->y ++;
	if (sd->y == 9) sd->y = 0;
	//puts ("Right");
	break;
      case 66:
	sd->x ++;
	if (sd->x == 9) sd->x = 0;
	//puts ("Down");
	break;
      deafult:
	break;
      }
    }
    sd->keycode = c;
  }
}

int field [9][9];
char expolred [9][9];

int solved (void) {
  return 0;
}

void update_screen (void) {
  int i,j,k;
  cn_pos (1,1);
  for (i=0; i<9; ++i) {
    for (j=0; j<9; ++j) {
      if (i == sd.x && j == sd.y)
	CSI ("7m");
      printf ("% 2d ", field[i][j]);
      CSI ("0m");
      if (j == 2 || j == 5) {
        printf ("| ");
      }
    }
    if (i == 2 || i == 5) {
      printf ("\n");
      for (k=0; k< 2*9 + 12; ++k)
	printf ("=");
    }
    printf ("\n");
  }

  // cn_pos (2, sd.x + 1 + sd.x/3);

  fflush (stdout);
}

#define RAND_LEVEL 0

void generate_field (void) {
  int i,j;

  for (i=0; i<9; ++i) {
    for (j=0; j<9; ++j) {
      field[i][j] = (i + j)%9 + 1;
    }
  }

  // randomization
  for (i=0; i < RAND_LEVEL; ++i) {
    int k = rand () % 9;
    int q = rand () % 9;

    int tt[9];
    for (j=0; j<8; j++) {
      tt[j + 1] = field[k][j];
    }
    tt[0] = field[k][8];

    for (j=0; j<9; ++j)
      field[k][j] = tt[j];

    for (j=1; j<9; j++) {
      tt[j - 1] = field[q][j];
    }
    tt[8] = field[q][0];

    for (j=0; j<9; ++j)
      field[q][j] = tt[j];

  }
}

void init () {
  // go int C break mode
  store_old ();
  set_raw_mode ();

  atexit (exxit);
  signal (SIGINT, handle_sigint);
  signal (SIGTERM, handle_sigterm);

  generate_field ();

  c_eraseScreen ();
  cn_pos (1,1);
  sd.x = 0;
  sd.y = 0;
  sd.field = (int*)field;

}

int main (void) {
  // initial print
  srand (time (NULL));
  init ();

  pthread_create (&pt, NULL, &handle_user_input, &sd);

  while (! solved ()) {
    // continue user input
    update_screen ();
    usleep (50000L);
  }

  return 0;
}
