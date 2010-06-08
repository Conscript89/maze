#ifndef LOGIC_H
#define LOGIC_H

#include <time.h>
#include <ncurses.h>

#define WALLS "|+-"

typedef struct {
  unsigned short int x;
  unsigned short int y;
} position;

typedef struct {
  short int width;
  short int height;
  position pos;
  char *data;
  _Bool keys[26];
  _Bool finished;
  int score;
  time_t start_time;
  time_t last_update;
  WINDOW *game_window;
  WINDOW *status_window;
  WINDOW *message_window;
} game;

typedef enum { UP, RIGHT, DOWN, LEFT } direction;

game * create_game();
_Bool init_game(game *g, short int width, short int height);
void reset_score(game *g);
void free_game(game *g);
void set_game_window(game *g, WINDOW *w);
void set_status_window(game *g, WINDOW *w);
void set_message_window(game *g, WINDOW *w);

void show_everything(game *g);
void show_visible(game *g);
void show_direction(game *g, direction d);
void show_pos(game *g, position pos);
void show_xy(game *g, short int x, short int y);

void update_status(game *g);
void show_message(game *g, const char msg[]);

_Bool blocks_pos(game g, position pos);
_Bool blocks_xy(game g, short int x, short int y);
_Bool blocks_view_pos(game g, position pos);
_Bool blocks_view_xy(game g, short int x, short int y);

char current_char(game g);
char current_char_pos(game g, position pos);
char current_char_xy(game g, short int x, short int y);

void clear_current(game *g);
void clear_pos(game *g, position pos);
void clear_xy(game *g, short int x, short int y);

void move_to_start(game *g);

_Bool can_move(game g, short int dx, short int dy);
_Bool try_move(game *g, short int dx, short int dy);
void do_move(game *g);

#endif
