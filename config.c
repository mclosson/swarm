#include <stdbool.h>
#include <stdio.h>
#include "config.h"

char *get_filename(struct config *config, int index)
{
  return *(config->files + index);
}

void init_config(struct config *config)
{
  config->debug = false;
  config->recursive = false;
  config->verbose = false;
  config->workers = 0;
  config->logfile = NULL;
  config->files = NULL;
  config->nfiles = 0;
}

void print_config(struct config *config)
{
  int i;

  printf("debug: %s\nworkers: %d\nverbose: %s\nlogfile: %s\n",
    config->debug ? "true" : "false",
    config->workers,
    config->verbose ? "true" : "false",
    config->logfile ? config->logfile : "<none>"
  );

  printf("wipefiles:");

  for (i = 0; i < config->nfiles; i++) {
    printf(" %s", *(config->files + i));
  }

  printf("\n");
}
