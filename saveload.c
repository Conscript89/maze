#include "saveload.h"

#ifdef DEBUG
#include <string.h>
#endif

_Bool load_file(FILE *f, game *g)
{
  short int width = 0;
  short int height = 0;

  // get width and height
  if (fscanf(f, "%hd,%hd", &width, &height) != 2)
    return 0;
  init_game(g, width, height);

  // load game data
  fseek(f, 1L, SEEK_CUR);
  for (int i = 0; i < height; i++) {
	for (int j = 0; j < width; j++) {
	  g->data[i*width+j] = fgetc(f);
	}
    fseek(f, 1L, SEEK_CUR);
  }
  g->data[(g->width)*(g->height)] = '\0';

  #ifdef DEBUG
  FILE *debugf = fopen("debug.dump", "w");
  fwrite(g->data, sizeof(char), strlen(g->data), debugf);
  fclose(debugf);
  #endif

  return 1;
}

/* maybe in future
_Bool save_file(FILE *f, game *p)
{
  return 0;
}
*/
