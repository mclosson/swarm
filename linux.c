#include <fcntl.h>
#include <unistd.h>

#include "errors.h"
#include "linux.h"

void arc4random_buf(void *buffer, size_t nbytes)
{
  int bytes_read, file_descriptor;

  file_descriptor = open("/dev/urandom", O_RDONLY);
  exit_if(file_descriptor == -1, OPEN_ERROR);

  bytes_read = read(file_descriptor, buffer, nbytes);
  exit_if(bytes_read == -1, READ_ERROR);

  exit_if(close(file_descriptor) == -1, CLOSE_ERROR);
}
