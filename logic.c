#include "logic.h"

#include <stdlib.h>
#include <string.h>
#include <ncurses.h>

_Bool init_game(game *g, short int width, short int height)
{
  if (g->data != NULL)
    return 0;
  if (g->keys == NULL) {
    return 0;
  }
  for (int i = 0; i < 26; i++)
	g->keys[i] = 0;
  g->data = (char *) malloc(width*height+1);
  if (g->data == NULL) {
    return 0;
  }
  g->data[0] = '\0';
  g->width = width;
  g->height = height;
  g->pos.x = 0;
  g->pos.y = 0;
  g->finished = 0;
  reset_score(g);
  return 1;
}

void reset_score(game *g)
{
  g->score = 0;
}

void free_game(game *g)
{
  if (g->data != NULL) {
	free(g->data);
	g->data = NULL;
  }
  g->width = g->height = 0;
}

void set_game_window(game *g, WINDOW *w)
{
  g->game_window = w;
}

void set_status_window(game *g, WINDOW *w)
{
  g->status_window = w;
}

void set_message_window(game *g, WINDOW *w)
{
  g->message_window = w;
}

void show_everything(game *g)
{
  if (g->data == NULL)
    return;
  for (int x = 0; x < g->width; x++)
	for (int y = 0; y < g->height; y++)
	  show_xy(g, x, y);
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
	   !blocks_view_pos(*g, pos);
       pos.x += dx, pos.y += dy) {
    for (int i = 0; i < 9; i++) {
      x = pos.x + (i%3 - 1);
      y = pos.y + (i/3 - 1);
	  if (x < 0 || x >= g->width || y < 0 || y >= g->height)
		continue;
	  show_xy(g, x, y);
    }
  }
  do_move(g);
}

void show_pos(game *g, position pos)
{
  show_xy(g, pos.x, pos.y);
}

void show_xy(game *g, short int x, short int y)
{
  char curch = g->data[y*(g->width) + x];
  wmove(g->game_window, y, x);
  if (blocks_xy(*g, x, y)) {
	// print normally
	waddch(g->game_window, curch);
  } else {
	// print colored
	wcolor_set(g->game_window, 2, NULL);
	waddch(g->game_window, curch);
	wcolor_set(g->game_window, 1, NULL);
  }
}

void update_status(game *g)
{
  char keys[27];
  for (int i = 0; i < 26; i++)
	keys[i] = g->keys[i]?('a'+i):' ';
  keys[26] = '\0';

  wclear(g->status_window);
  wmove(g->status_window, 0, 1);
  wprintw(g->status_window, "Score: %d Keys: %s Time: ", g->score, keys);
  wrefresh(g->status_window);
  do_move(g);
}

void show_message(game *g, const char msg[])
{
  wclear(g->message_window);
  wmove(g->message_window, 0, 1);
  waddstr(g->message_window, msg);
  wrefresh(g->message_window);
  do_move(g);
}

_Bool blocks_pos(game g, position pos)
{
  return blocks_xy(g, pos.x, pos.y);
}

_Bool blocks_xy(game g, short int x, short int y)
{
  char curch = current_char_xy(g, x, y);
  if (x >= g.width || y >= g.height)
	return 1;
  if (strchr(WALLS, curch) != NULL)
    return 1;
  if (curch >= 'A' && curch <= 'Z' && g.keys[curch-'A'] == 0)
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
  // pick up key
  if (curch >= 'a' && curch <= 'z') {
	char msg[] = "Picked up key  ";
	msg[strlen(msg)-1] = curch;
    g->keys[curch-'a'] = 1;
    g->score += 100;
    clear_current(g);
	show_message(g, msg);
  }
  // open door
  if (curch >= 'A' && curch <= 'Z') {
	char msg[] = "Opened door  ";
	msg[strlen(msg)-1] = curch;
    clear_current(g);
    g->keys[curch-'A'] = 0;
	show_message(g, msg);
  }
  // pick up bonus
  if (curch == '*') {
    g->score += 1000;
    clear_current(g);
	show_message(g, "Found bonus!");
  }
  // starting position
  if (curch == '^')
    clear_current(g);
  // finish
  if (curch == '$') {
    clear_current(g);
    g->finished = 1;
	show_message(g, "Congratulation, you have found exit!");
  }

  wmove(g->game_window, g->pos.y, g->pos.x);
}
