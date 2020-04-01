#ifndef UNNAMED_FUNCTIONS_H_INCLUDED
#define UNNAMED_FUNCTIONS_H_INCLUDED

#include <errno.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/types.h>
#include <stdio.h>

#include "simpledu_args.h"

int simpledu_iterate(const char *path, simpledu_args_t arg, char *envp[], int *pipe_id, off_t *size);


#endif //UNNAMED_FUNCTIONS_H_INCLUDED