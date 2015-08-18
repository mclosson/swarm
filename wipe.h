#ifndef WIPE_H
#define WIPE_H

#define BLOCKSIZE 4096

static int ignore_entry(struct stat *, struct dirent *);
static void overwrite_bytes(int file_descriptor, int size, void (*generate)(void *, size_t));
void overwrite_file(int file_descriptor, int size, void (*generate)(void *, size_t));
void wipe_directory_tree(struct config *, const char *);
void wipe_file(const char *filename);

#endif
