#define _XOPEN_SOURCE 700
#include "simpledu_signals.h"

#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "arg.h"
#include "simpledu_args.h"
#include "simpledu_log.h"

static pid_t children_group;
void initialize_data(pid_t p) { children_group = p; }

int simpledu_handler() {
    struct sigaction action;
    sigset_t sigmask;
    pid_t pgid = arg.children_process_group;
    // char temp[999];
    // sprintf(temp, "HANDLER INST %d/%d\n", getpgrp(), pgid);
    // write(1, temp, strlen(temp));
    if (pgid == 0) {  // First process, will handle second group
        action.sa_handler = sigint_handler;
        sigemptyset(&action.sa_mask);
        action.sa_flags = SA_RESTART;
        if (sigaction(SIGINT, &action, NULL) == -1) {
            return EXIT_FAILURE;
        }
    } else {  // Process in second group, no special handling apart from
              // blocking SIGINT
        sigemptyset(&action.sa_mask);
        sigaddset(&sigmask, SIGINT);
        if (sigprocmask(SIG_BLOCK, &sigmask, NULL) == -1) {
            return EXIT_FAILURE;
        }
    }

    return EXIT_SUCCESS;
}

void sigint_handler(int signo) {
    pid_t pgid = children_group;

    char input[PATH_MAX] = {0};
    char output[PATH_MAX];
    if (pgid == 0) {
        return;
    }

    if (signo == SIGINT) {
        if (simpledu_killpg(pgid, SIGSTOP)) return;
        sprintf(output, "Are you sure you want to terminate the program? (y/n)\n");
        write(STDERR_FILENO, output, strlen(output));

        read(STDIN_FILENO, input, 1);
        if (*input == 'n' || *input == 'N') {
            if (simpledu_killpg(pgid, SIGCONT)) return;
        } else /*if (*input == 'y' || *input == 'Y')*/ {
            if (simpledu_killpg(pgid, SIGTERM)) return;
            if (simpledu_killpg(0, SIGTERM)) return;
        }
    }
}