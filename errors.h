#ifndef ERRORS_H
#define ERRORS_H

#define CLOSE_ERROR "Error closing file"
#define FORK_ERROR "Error on fork"
#define MALLOC_ERROR "Error allocating memory"
#define OPEN_ERROR "Error opening file"
#define OPENDIR_ERROR "Error opening directory"
#define PIPE_ERROR "Error opening pipe"
#define READ_ERROR "Error reading file"
#define REALPATH_ERROR "Error on realpath"
#define SELECT_ERROR "Error on select"
#define STAT_ERROR "Error on stat"
#define UNLINK_ERROR "Error unlinking file"
#define USAGE_ERROR "Usage: swarm <files>"
#define WRITE_ERROR "Error writing to file"

void exit_if(int condition, const char *message);

#endif
