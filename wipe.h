#ifndef WIPE_H
#define WIPE_H

#define BLOCKSIZE 4096

static void overwrite_bytes(int file_descriptor, int size, void (*generate)(void *, size_t));
void overwrite_file(int file_descriptor, int size, void (*generate)(void *, size_t));
void wipe_file(const char *filename);

#endif
