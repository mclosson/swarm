#include <errno.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "errors.h"

void exit_if(int condition, const char *format, ...)
{
  va_list arglist;

  if (!condition) { return; }

  va_start(arglist, format);
  vfprintf(stderr, format, arglist);
  fprintf(stderr, ": %s\n", strerror(errno));
  va_end(arglist);

  exit(EXIT_FAILURE);
}
