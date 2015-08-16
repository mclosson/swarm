#ifndef CONFIG_H
#define CONFIG_H

struct config {
  bool debug;
  bool recursive;
  bool verbose;
  char *logfile;
  char **files;
  int nfiles;
  int workers;
};

char *get_filename(struct config *, int);
void init_config(struct config *);
void print_config(struct config *);

#endif
