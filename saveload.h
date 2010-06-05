#ifndef SAVELOAD_H
#define SAVELOAD_H

#include <stdio.h>
#include "logic.h"

_Bool load_file(FILE *f, game *g);
_Bool save_file(FILE *f, game *g);

#endif
