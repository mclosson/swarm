#include <stdio.h>
#include <stdlib.h>
#include "errors.h"

void exit_if(int condition, const char *message)
{
  if (!condition) { return; }
  fprintf(stderr, "%s\n", message);
  exit(EXIT_FAILURE);
}
