#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <errno.h>

#include "simpledu_args.h"
#include "simpledu_envp.h"
#include "simpledu_time.h"
#include "simpledu_log.h"
#include "simpledu_iterate.h"

#define FILEDES_INVALID -1

simpledu_args_t arg;
simpledu_envp_t env;

int simpledu_init(int argc, char *argv[], char *envp[]){
    //Arguments and environment
    if(simpledu_args_ctor(&arg, argc, argv) ||
       simpledu_envp_ctor(&env, envp)) return EXIT_FAILURE;

    //
    return EXIT_SUCCESS;
}

void simpledu_clean(){
    simpledu_envp_dtor(&env);
    simpledu_args_dtor(&arg);
}

int main(int argc, char *argv[], char *envp[]){

    if(simpledu_init(argc, argv, envp)) simpledu_exit(EXIT_FAILURE);
    if(atexit(simpledu_clean)) simpledu_exit(EXIT_FAILURE);

    //Time
    if(arg.start_time == -1){
        if(simpledu_starttime(&arg.start_time)) simpledu_exit(EXIT_FAILURE);
    } else {
        if(simpledu_set_starttime(arg.start_time)) simpledu_exit(EXIT_FAILURE);
    }
    // Log file descriptor
    if(arg.log_filedes != FILEDES_INVALID){
        //A parent process provided a log filedes
        //Case 2
        if(simpledu_log_setfiledes(arg.log_filedes)) simpledu_exit(EXIT_FAILURE);
    } else {
        //No filedes provided
        //Cases 1, 3, 4
        if(env.LOG_FILENAME != NULL){
            //No filedes, but log; can only happen to parent with LOG_FILENAME set
            //Case 1
            arg.log_filedes = simpledu_log_createlog(env.LOG_FILENAME);
            if(arg.log_filedes == FILEDES_INVALID) simpledu_exit(EXIT_FAILURE);
        } else{
            //No log; we do nothing, given that simpledu_log utilities default is to do nothing
            //Cases 3, 4
        }
    }

    // Create log
    if(simpledu_log_CREATE(argc, (const char * const*)argv)) simpledu_exit(EXIT_FAILURE);

    // DO NOT UNCOMMENT
    
    /*
    int pid = fork();
    if(pid > 0){ //parent

    }else if(pid == 0){ //child
        char path[PATH_MAX];
        if(getcwd(path, PATH_MAX) == NULL) simpledu_exit(EXIT_FAILURE);
        strcat(path, "/simpledu");
        char **new_argv = NULL;
        if(simpledu_args_toargv(&arg, &new_argv)) simpledu_exit(EXIT_FAILURE);
        if(execve(path, new_argv, envp)) simpledu_exit(EXIT_FAILURE);
    }else{
        simpledu_exit(EXIT_FAILURE);
    }
    */

    for(size_t i = 0; i < arg.filesc; ++i){
        int pipe_id;
        off_t size, more_size;

        if(simpledu_iterate(arg.files[i], arg, envp, &pipe_id, &size)) simpledu_exit(EXIT_FAILURE);
        if(simpledu_retrieve(arg.pipe_filedes, &more_size));
        if(simpledu_print(arg.files[i], size, more_size, arg));
    }

    simpledu_exit(EXIT_SUCCESS);
}
