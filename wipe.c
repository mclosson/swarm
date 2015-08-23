#include <sys/stat.h>

#include <dirent.h>
#include <fcntl.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "config.h"
#include "errors.h"
#include "wipe.h"

static int ignore_entry(struct stat *stat, struct dirent *dirent)
{
  int isdot, isdotdot, length;

  length = strlen(dirent->d_name);
  isdot = strncmp(dirent->d_name, ".", length) == 0;
  isdotdot = strncmp(dirent->d_name, "..", length) == 0;

  return S_ISDIR(stat->st_mode) && (isdot || isdotdot);
}

static void overwrite_bytes(int file_descriptor, int size, void (*generate)(void *, size_t))
{
  char buffer[size];
  int wrote;

  generate(buffer, size);
  wrote = write(file_descriptor, buffer, size);
  exit_if(wrote != size, WRITE_ERROR);
}

void overwrite_file(int file_descriptor, int size, void (*generate)(void *, size_t))
{
  int block, blocks, bytes, result;

  blocks = size / BLOCKSIZE;
  bytes = size % BLOCKSIZE;

  result = lseek(file_descriptor, 0, SEEK_SET);
  exit_if(result == -1, READ_ERROR);

  for (block = 0; block < blocks; block++) {
    overwrite_bytes(file_descriptor, BLOCKSIZE, generate);
  }

  overwrite_bytes(file_descriptor, bytes, generate);
}

void wipe_directory_tree(struct config *config, const char *name)
{
  DIR *dir;
  struct dirent *dirent;
  struct stat filestat;
  char fullpath[FILENAME_MAX];
  char filename[FILENAME_MAX];

  exit_if(realpath(name, fullpath) == NULL, REALPATH_ERROR);

  dir = opendir(fullpath);
  exit_if(dir == NULL, OPENDIR_ERROR);

  while ((dirent = readdir(dir)) != NULL) {

    snprintf(filename, sizeof filename, "%s/%s", fullpath, dirent->d_name);
    stat(filename, &filestat);
    if (ignore_entry(&filestat, dirent)) { continue; }

    if (S_ISDIR(filestat.st_mode)) {
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

  closedir(dir);
}

void wipe_file(const char *filename)
{
  int file_descriptor, file_size, passes;

  file_descriptor = open(filename, O_WRONLY|O_SYNC);
  exit_if(file_descriptor == -1, OPEN_ERROR);

  file_size = lseek(file_descriptor, 0, SEEK_END);
  exit_if(file_size == -1, READ_ERROR);

  for (passes = 0; passes < 8; passes++) {
    overwrite_file(file_descriptor, file_size, arc4random_buf);
  }

  overwrite_file(file_descriptor, file_size, bzero);

  exit_if(close(file_descriptor) == -1, CLOSE_ERROR);
  exit_if(unlink(filename) == -1, UNLINK_ERROR);
}
