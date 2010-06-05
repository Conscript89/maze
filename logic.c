#include "logic.h"

#include <stdlib.h>
#include <string.h>
#include <ncurses.h>

_Bool init_game(game *g, short int width, short int height)
{
  if (g->data != NULL)
    return 0;
  g->keys = (_Bool *) malloc(sizeof(_Bool)*26); // 26 is number of characters
  if (g->keys == NULL) {
    return 0;
  }
  g->data = (char *) malloc(width*height+1);
  if (g->data == NULL) {
    free(g->keys);
    g->keys = NULL;
    return 0;
  }
  g->data[0] = '\0';
  g->width = width;
  g->height = height;
  g->pos.x = 0;
  g->pos.y = 0;
  g->score = 0;
  g->finished = 0;
  return 1;
}

void free_game(game *g)
{
  if (g->data == NULL)
    return;
  free(g->data);
  g->data = NULL;
  free(g->keys);
  g->keys = NULL;
  g->width = g->height = 0;
}

void show_everything(game *g)
{
  if (g->data == NULL)
    return;
  clear();
  move(0, 0);
  for (int i = 0; i < g->height; i++) {
    move(i, 0);
    addnstr(g->data+(i*(g->width)), g->width);
  }
}

void show_visible(game *g)
{
  show_direction(g, UP);
  show_direction(g, RIGHT);
  show_direction(g, LEFT);
  show_direction(g, DOWN);
}

void show_direction(game *g, direction d)
{
  short int dx,x;
  short int dy,y;
  char curch;
  dx = x = dy = y = 0;
  switch (d) {
  case UP:
    dy--;
    break;
  case RIGHT:
    dx++;
    break;
  case DOWN:
    dy++;
    break;
  case LEFT:
    dx--;
    break;
  }
  for (position pos = g->pos;
       pos.x >= 0 && pos.x < g->width && pos.y >= 0 && pos.y < g->height;
       pos.x += dx, pos.y += dy) {
    if (blocks_view_pos(*g, pos))
      break;
    curch = g->data[pos.y*(g->width) + pos.x];
    for (int i = 0; i < 9; i++) {
      x = pos.x + (i%3 - 1);
      y = pos.y + (i/3 - 1);
      curch = g->data[y*(g->width) + x];
      move(y, x);
      addch(curch);
    }
  }
  do_move(g);
}

_Bool blocks_pos(game g, position pos)
{
  return blocks_xy(g, pos.x, pos.y);
}

_Bool blocks_xy(game g, short int x, short int y)
{
  char curch = current_char_xy(g, x, y);
  if (strchr(WALLS, curch) != NULL)
    return 1;
  if (curch >= 'A' && curch <= 'Z' && g.keys['A'-curch] == 0)
    return 1;
  return 0;
}

_Bool blocks_view_pos(game g, position pos)
{
  return blocks_view_xy(g, pos.x, pos.y);
}

_Bool blocks_view_xy(game g, short int x, short int y)
{
  char curch = current_char_xy(g, x, y);
  if (blocks_xy(g, x, y) || (curch >= 'A' && curch <= 'Z'))
    return 1;
  return 0;  
}

char current_char(game g)
{
  return current_char_xy(g, g.pos.x, g.pos.y);
}

char current_char_pos(game g, position pos)
{
  return current_char_xy(g, pos.x, pos.y);
}

char current_char_xy(game g, short int x, short int y)
{
  return g.data[y*(g.width) + x];
}

void clear_current(game *g)
{
  clear_xy(g, g->pos.x, g->pos.y);
}

void clear_pos(game *g, position pos)
{
  clear_xy(g, pos.x, pos.y);
}

void clear_xy(game *g, short int x, short int y)
{
  g->data[y*(g->width) + x] = ' ';
}

void move_to_start(game *g)
{
  char *startpos = strchr(g->data, '^');
  int relpos;
  if (startpos == NULL)
    return;
  relpos = startpos-(g->data);
  g->pos.x = relpos%(g->width);
  g->pos.y = relpos/(g->width);
  do_move(g);
}

_Bool can_move(game g, short int dx, short int dy)
{
  short int x = (g.pos.x) + dx;
  short int y = (g.pos.y) + dy;
  if (g.data == NULL)
    return 0;
  if (x < 0 || y < 0)
    return 0;
  if (blocks_xy(g, x, y))
    return 0;
  return 1;
}

_Bool try_move(game *g, short int dx, short int dy)
{
  if (can_move(*g, dx, dy)) {
    g->pos.x += dx;
    g->pos.y += dy;
    do_move(g);
    return 1;
  } else
    return 0;
}

void do_move(game *g)
{
  char curch = current_char(*g);
  if (curch >= 'a' && curch <= 'z') {
    g->keys['a'-curch] = 1;
    g->score += 100;
    clear_current(g);
  }
  if (curch >= 'A' && curch <= 'Z') {
    clear_current(g);
    g->keys['A'-curch] = 0;
  }
  if (curch == '*') {
    g->score += 1000;
    clear_current(g);
  }
  if (curch == '^')
    clear_current(g);
  if (curch == '$') {
    clear_current(g);
    g->finished = 1;
  }
  move(g->pos.y, g->pos.x);
}
