#include <stdlib.h>
#include <unistd.h>

#include "errors.h"
#include "pipeset.h"

void close_pipes(int *pipes, int size, int leave_open)
{
  int pipe;

  for (int a = 0; a < size; a++) {
    for (int b = 0; b < 2; b++) {
      pipe = *(pipes + a * 2 + b);
      if (pipe != leave_open) {
        exit_if(close(pipe) == -1, CLOSE_ERROR);
      }
    }
  }
}

int get_pipe(int *pipes, int index, int io)
{
  return *(pipes + (index * 2 + io));
}

struct pipeset *init_pipeset(int count)
{
  int i;
  struct pipeset *pipes;

  pipes = malloc(sizeof (struct pipeset));
  exit_if(pipes == NULL, MALLOC_ERROR);

  pipes->requestpipes = malloc(count * 2 * sizeof (int));
  exit_if(pipes->requestpipes == NULL, MALLOC_ERROR);
  pipes->responsepipes = malloc(count * 2 * sizeof (int));
  exit_if(pipes->responsepipes == NULL, MALLOC_ERROR);
  pipes->count = count;

  for (i = 0; i < count; i++) {
    exit_if(pipes->responsepipes[i] == -1, PIPE_ERROR);
    exit_if(pipes->requestpipes[i] == -1, PIPE_ERROR);
  }

  return pipes;
}
