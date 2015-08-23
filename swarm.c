/* SwaRM: parallelized secure delete application */

#include <sys/select.h>
#include <sys/stat.h>

#include <dirent.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "config.h"
#include "errors.h"
#include "pipeset.h"
#include "wipe.h"

static void childloop(struct config *config, int requestpipe, int responsepipe)
{
  int wrote;
  char filename[BLOCKSIZE];
  struct stat filestat;

  while (read(requestpipe, filename, sizeof filename) != -1) {
    exit_if(stat(filename, &filestat) == -1, STAT_ERROR);

    if (S_ISDIR(filestat.st_mode) && config->recursive) {
      wipe_directory_tree(config, filename);
    }
    else {
      if (config->verbose) { printf("wiping: %s...", filename); }
      if (!config->debug) { wipe_file(filename); }
      if (config->verbose) { printf("complete.\n"); }
    }

    wrote = write(responsepipe, ".", 2);
    exit_if(wrote == -1, WRITE_ERROR);
  }
}

static void parentloop(struct config *config, struct pipeset *pipes)
{
  char *filename;
  char buffer[BLOCKSIZE];
  int i, ready, readpipe, writepipe, wrote, queued;
  struct fd_set readset;

  queued = config->nfiles;

  while (queued) {
    FD_ZERO(&readset);

    for (i = 0; i < pipes->count; i++) {
      readpipe = get_pipe(pipes->requestpipes, i, 0);
      FD_SET(readpipe, &readset);
    }

    ready = select(FD_SETSIZE, &readset, NULL, NULL, NULL);
    exit_if(ready == -1, SELECT_ERROR);

    for (i = 0; i < pipes->count; i++) {
      readpipe = get_pipe(pipes->requestpipes, i, 0);
      writepipe = get_pipe(pipes->responsepipes, i, 1);

      if (FD_ISSET(readpipe, &readset)) {
        exit_if(read(readpipe, buffer, sizeof buffer) == -1, READ_ERROR);

        if (queued) {
          filename = get_filename(config, config->nfiles - queued);
          wrote = write(writepipe, filename, strlen(filename) + 1);
          /* TODO: Need a way to verify the file was unlinked before dequeuing it */
          if (wrote == strlen(filename) + 1) { queued--; }
        }
      }
    }
  }
}

static void serial_wipe_files(struct config *config)
{
  int i;
  char *filename;
  struct stat filestat;

  for (i = 0; i < config->nfiles; i++) {
    filename = get_filename(config, i);
    exit_if(stat(filename, &filestat) == -1, STAT_ERROR);

    if (S_ISDIR(filestat.st_mode) && config->recursive) {
      wipe_directory_tree(config, filename);

      if (config->verbose) { printf("removing: %s", filename); }
      if (!config->debug) { rmdir(filename); }
      if (config->verbose) { printf("\n"); }
    }
    else {
      if (config->verbose) { printf("wiping: %s", filename); }
      if (!config->debug) { wipe_file(filename); }
      if (config->verbose) { printf("\n"); }
    }
  }
}

static void print_usage(const char *name)
{
  fprintf(stderr, "Usage: %s [-d] [-r] [-v] [-w workers] [-o logfile] <files>\n", name);
}

int main(int argc, char *argv[])
{
  int child, opt, requestpipe, responsepipe;
  pid_t pid;
  struct config config;
  struct pipeset *pipes;

  if (argc < 2) {
    print_usage(argv[0]);
    exit(EXIT_FAILURE);
  }

  init_config(&config);

  while ((opt = getopt(argc, argv, "drw:vo:")) != -1) {
    switch (opt) {
      case 'd':
        config.debug = true;
        break;
      case 'r':
        config.recursive = true;
        break;
      case 'w':
        config.workers = atoi(optarg);
        break;
      case 'v':
        config.verbose = true;
        break;
      case 'o':
        config.logfile = optarg;
        break;
      case '?':
      default:
        print_usage(argv[0]);
        exit(EXIT_FAILURE);
    }
  }

  config.nfiles = argc - optind;
  config.files = argv + optind;

  if (config.debug) { print_config(&config); }

  if (config.workers == 0) {
    serial_wipe_files(&config);
    exit(EXIT_SUCCESS);
  }

  pipes = init_pipeset(config.workers);

  for (child = 0; child < config.workers; child++) {
    pid = fork();
    exit_if(pid == -1, FORK_ERROR);

    if (pid) { /* Child */
      requestpipe = get_pipe(pipes->requestpipes, child, 1);
      responsepipe = get_pipe(pipes->responsepipes, child, 0);

      close_pipes(pipes->requestpipes, config.workers, requestpipe);
      close_pipes(pipes->responsepipes, config.workers, responsepipe);

      childloop(&config, requestpipe, responsepipe);
      exit(EXIT_SUCCESS);
    }
    else { /* Parent */
      exit_if(close(get_pipe(pipes->responsepipes, child, 0)) == -1, CLOSE_ERROR);
      exit_if(close(get_pipe(pipes->requestpipes, child, 1)) == -1, CLOSE_ERROR);
    }
  }

  parentloop(&config, pipes);
  exit(EXIT_SUCCESS);
}
