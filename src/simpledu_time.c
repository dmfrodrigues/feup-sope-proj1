#include "simpledu_time.h"

#include <stdlib.h>
#include <time.h>
#include <errno.h>

static clock_t start_time;
static double ticks;

int simpledu_starttime(){
    start_time = clock();
    if(start_time == -1) return EXIT_FAILURE;
    ticks = CLOCKS_PER_SEC/1000.0;
    return EXIT_SUCCESS;
}

int simpledu_gettime(double *d){
    if(d == NULL){
        errno = EINVAL;
        return EXIT_FAILURE;
    }
    clock_t now = clock();
    if(now == -1) return EXIT_FAILURE;
    *d = (double)(now - start_time)/ticks;
    return EXIT_SUCCESS;
}
