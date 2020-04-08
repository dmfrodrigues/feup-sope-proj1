#define _XOPEN_SOURCE 700
#include "simpledu_signals.h"

#include "simpledu_args.h"
#include "arg.h"

#include <stdio.h> 
#include <signal.h>
#include <unistd.h>
#include <string.h>

static pid_t children_group;
void initialize_data(pid_t p){
    children_group = p;
}

int simpledu_handler(){
    struct sigaction action;
    sigset_t sigmask;
    pid_t pgid = arg.children_process_group;
    // char temp[999];
    // sprintf(temp, "HANDLER INST %d/%d\n", getpgrp(), pgid);
    // write(1, temp, strlen(temp));
    if (pgid == 0) { //First process, will handle second group
        action.sa_handler = sigint_handler;
        sigemptyset(&action.sa_mask);
        action.sa_flags = 0;
        if (sigaction(SIGINT,&action,NULL) == -1) {
            return EXIT_FAILURE;
        }
    } 
    else { //Process in second group, no special handling apart from blocking SIGINT
        sigemptyset(&action.sa_mask);
        sigaddset(&sigmask, SIGINT);
        if (sigprocmask(SIG_BLOCK, &sigmask, NULL) == -1){
            return EXIT_FAILURE;
        }
    }

    return EXIT_SUCCESS;
}

void sigint_handler(int signo){
    pid_t pgid = children_group;

    char x[999];
    sprintf(x, "IN HANDLER -> %d/%d\t", getpgrp(), pgid);
    write(1, x, strlen(x));

    char input[PATH_MAX];
    if (pgid == 0) {
        return;
    }

    if (signo == SIGINT){
        printf("\tPGIDCHILD %d\n", pgid);
        killpg(pgid, SIGSTOP);
        
            printf("Are you sure you want to terminate the program? (y/n)\n");
            if (scanf("%c", input) != 1) return;
            if (*input == 'n' || *input == 'N'){
                killpg(pgid, SIGCONT);
            } else /*if (*input == 'y' || *input == 'Y')*/{
                killpg(pgid, SIGTERM);
            }
    }
}