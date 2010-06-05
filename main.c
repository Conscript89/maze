#include <ncurses.h>
#include <time.h>
#include <stdio.h>

#include "logic.h"
#include "saveload.h"

_Bool init();
_Bool initcolors();
void mainloop(game *g);
void cleanup();
void show_error(const char msg[]);

int main(int argc, char *argv[])
{
  int retval = 0;
  game g;
  g.data = NULL;
  
  if (argc < 2)
    retval = 1;

  if (!init())
    retval = 2;
  else
    for (int i = 1; i < argc; i++) {
      FILE *f = fopen(argv[i], "r");
      if (load_file(f, &g)) {
	fclose(f);
	mainloop(&g);
	free_game(&g);
	clear();
	refresh();
      } else
	show_error("File couldn't be opened or has incorrect format!");
    }
  cleanup();

  return retval;
}

_Bool init()
{
  initscr();
  cbreak();
  noecho();
  keypad(stdscr, 1);
  //timeout(1000);
  nl();
  clear();
  curs_set(1);
  return 1;
}

_Bool initcolors()
{
  /*
  if (has_colors()) {
    start_color();
    init_pair(1, FG, BG);
    start_color();
    color_set(1, NULL);
  }
  */
  return 1;
}

void mainloop(game *g)
{
  _Bool ended = 0;
  int key;
  position pos;
  short int dx;
  short int dy;
  pos.x = 0;
  pos.y = 0;
  //show_everything(g);
  move_to_start(g);
  while (!ended && !g->finished) {
    show_visible(g);
    dx = 0;
    dy = 0;
    key = getch();
    switch (key) {
    case 'q':
      ended = 1;
      break;
    case KEY_LEFT:
      dx--;
      break;
    case KEY_RIGHT:
      dx++;
      break;
    case KEY_UP:
      dy--;
      break;
    case KEY_DOWN:
      dy++;
      break;
    default:
      break;
    }
    try_move(g, dx, dy);
    refresh();
  }
}

void cleanup()
{
  clear();
  endwin();
}

void show_error(const char msg[])
{
  clear();
  move(12, 5);
  insstr(msg);
  getch();
}
