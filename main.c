#include <ncurses.h>
#include <time.h>
#include <stdio.h>

#include "logic.h"
#include "saveload.h"

#define FG COLOR_WHITE
#define BG COLOR_BLACK

_Bool init();
_Bool initcolors();
void mainloop(game *g);
void cleanup();
void show_error(WINDOW *w, const char msg[]);

int main(int argc, char *argv[])
{
  int retval = 0;
  game g;
  
  if (argc < 2)
    retval = 1;

  if (!init())
    retval = 2;
  else {
	set_game_window(&g, newwin(23, 80, 1, 0));
	set_status_window(&g, newwin(1, 80, 0, 0));
	set_message_window(&g, newwin(1, 80, 24, 0));
	keypad(g.game_window, 1);
    for (int i = 1; i < argc; i++) {
      FILE *f = NULL;
      if ((f = fopen(argv[i], "r")) != NULL && load_file(f, &g)) {
		fclose(f);	
		mainloop(&g);
		free_game(&g);
		wclear(g.game_window);
		wrefresh(g.game_window);
      } else
		show_error(g.message_window,
				   "File couldn't be opened or has incorrect format!");
    }
  }
  cleanup();

  return retval;
}

_Bool init()
{
  initscr();
  cbreak();
  noecho();
  timeout(1000);
  nl();
  clear();
  curs_set(1);
  initcolors();
  return 1;
}

_Bool initcolors()
{
  if (has_colors()) {
    start_color();
    init_pair(1, FG, BG);
	init_pair(2, BG, FG);
    start_color();
    color_set(1, NULL);
  }
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
  #ifdef DEBUG
    show_everything(g);
  #endif
  move_to_start(g);
  while (!ended && !g->finished) {
    show_visible(g);
	update_status(g);
    dx = 0;
    dy = 0;
    key = wgetch(g->game_window);
    switch (key) {
    case 'q':
      ended = 1;
      break;
    case KEY_LEFT:
	case 'h':
      dx--;
      break;
    case KEY_RIGHT:
	case 'l':
      dx++;
      break;
    case KEY_UP:
	case 'k':
      dy--;
      break;
    case KEY_DOWN:
	case 'j':
      dy++;
      break;
    default:
      break;
    }
    try_move(g, dx, dy);
    wrefresh(g->game_window);
  }
}

void cleanup()
{
  clear();
  endwin();
}

void show_error(WINDOW *w, const char msg[])
{
  wclear(w);
  wmove(w, 0, 0);
  winsstr(w, msg);

  wrefresh(w);
  timeout(-1);
  getch();
  timeout(1000);
}
