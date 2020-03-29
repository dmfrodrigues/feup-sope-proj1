#ifndef UNNAMED_FUNCTIONS_H_INCLUDED
#define UNNAMED_FUNCTIONS_H_INCLUDED

#include <errno.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/types.h>
#include <stdio.h>

#include "simpledu_args.h"

int simpledu_iterate(const char *path, int *pipe_pid, simpledu_args_t arg, char *envp[]);

#endif //UNNAMED_FUNCTIONS_H_INCLUDED