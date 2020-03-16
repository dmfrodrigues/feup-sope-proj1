#include <stdlib.h>
#include <stdio.h>

#include "simpledu_args.h"

int main(int argc, char *argv[], char *envp[]){
    simpledu_args_t p;
    simpledu_args_ctor(&p, argc-1, &argv[1]);
    printf("%d\n", p.block_size);
    return 0;
}
