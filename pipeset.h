#ifndef PIPESET_H
#define PIPESET_H

struct pipeset {
  int *requestpipes;
  int *responsepipes;
  int count;
};

void close_pipes(int *, int, int);
int get_pipe(int *, int, int);
struct pipeset *init_pipeset(int);

#endif
