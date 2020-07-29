#include <stdlib.h>
#include <time.h>
#include "utils.h"

float randf()
{
  srand(time(NULL));
  return rand() / (float)RAND_MAX;
}

Vec randVec()
{
  return Vec(randf(), randf(), randf());
}

Vec randPositionInCube(Vec center, int edge)
{
  Vec position;
  position[0] = (edge / 2.0) * randf() + center[0];
  position[1] = (edge / 2.0) * randf() + center[1];
  position[2] = (edge / 2.0) * randf() + center[2];
  return position;
}
