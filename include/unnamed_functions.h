#ifndef UNNAMED_FUNCTIONS_H_INCLUDED
#define UNNAMED_FUNCTIONS_H_INCLUDED

#include <errno.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/types.h>

int simpledu_iterate(const char *path, int *pipe_pid);

#endif //UNNAMED_FUNCTIONS_H_INCLUDED