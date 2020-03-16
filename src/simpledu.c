#include <stdlib.h>
#include <stdio.h>

#include "simpledu_args.h"

int main(int argc, char *argv[], char *envp[]){
    simpledu_args_t p;
    simpledu_args_ctor(&p, argc-1, (const char**)&argv[1]);
    return 0;
}
