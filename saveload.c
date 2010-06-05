#include "saveload.h"

_Bool load_file(FILE *f, game *g)
{
  short int width = 0;
  short int height = 0;
  if (fscanf(f, "%hd,%hd\n", &width, &height) != 2)
    return 0;
  init_game(g, width, height);
  for (int i = 0; i < height; i++) {
    fread(g->data+(i*width), width, 1, f);
    fseek(f, 1L, SEEK_CUR);
  }
  g->data[(g->width)*(g->height)] = '\0';
  return 1;
}

_Bool save_file(FILE *f, game *p)
{
  return 0;
}
