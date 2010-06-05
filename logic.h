#ifndef LOGIC_H
#define LOGIC_H

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
  _Bool *keys;
  _Bool finished;
  int score;
} game;

typedef enum { UP, RIGHT, DOWN, LEFT } direction;

_Bool init_game(game *g, short int width, short int height);
void free_game(game *g);

void show_everything(game *g);
void show_visible(game *g);
void show_direction(game *g, direction d);

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
