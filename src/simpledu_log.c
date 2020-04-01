#include "simpledu_log.h"

#include "simpledu_time.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <signal.h>

#define MAXT    64
#define MAXN    4096

#define NOFILEDES -1

static int filedes = NOFILEDES;

int simpledu_log_createlog(const char * filename){
    filedes = creat(filename, S_IRWXU);
    return filedes;
}

int simpledu_log_setfiledes(int fildes){
    filedes = fildes;
    return EXIT_SUCCESS;
}

ssize_t simpledu_log_write(const void *buf, size_t count){
    if(filedes == NOFILEDES) return 0;
    return write(filedes, buf, count);
}

/**
 * @brief Log an EXIT action.
 * 
 * Used to log just before a process exits.
 * 
 * @param status    Status the process will exit with
 * @return int      EXIT_SUCCESS if successful, EXIT_FAILURE otherwise
 */
int simpledu_log_EXIT(int status){
    if(filedes == -1) return EXIT_SUCCESS;
    double t; if(simpledu_gettime(&t)) return EXIT_FAILURE; 
    char buff[MAXN]; if(sprintf(buff, "%.2f %08d EXIT %d\n", t, getpid(), status) < 0) return EXIT_FAILURE;
    if(simpledu_log_write(buff, strlen(buff)) == -1) return EXIT_FAILURE;
    return EXIT_SUCCESS;
}

/**
 * @brief Log a SEND_SIGNAL action.
 * 
 * Should be used by the process sending a signal, just before being sent.
 * 
 * @param sig   Signal sent
 * @param pid   Process ID to which the signal is being sent
 * @return int  EXIT_SUCCESS if successful, EXIT_FAILURE otherwise
 */
int simpledu_log_SEND_SIGNAL(int sig, pid_t pid){
    if(filedes == -1) return EXIT_SUCCESS;
    double t; if(simpledu_gettime(&t)) return EXIT_FAILURE; 
    char buff[MAXN]; if(sprintf(buff, "%.2f %08d SEND_SIGNAL %d %d\n", t, getpid(), sig, pid) < 0) return EXIT_FAILURE;
    if(simpledu_log_write(buff, strlen(buff)) == -1) return EXIT_FAILURE;
    return EXIT_SUCCESS;
}

int simpledu_log_CREATE(int argc, const char* const argv[]){
    if(filedes == -1) return EXIT_SUCCESS;
    double t; if(simpledu_gettime(&t)) return EXIT_FAILURE; 
    char buff[MAXN]; if(sprintf(buff, "%.2f %08d CREATE ", t, getpid()) < 0) return EXIT_FAILURE;
    for(int i = 0; i < argc; ++i) strcat(strcat(buff, argv[i]), " ");
    strcat(buff, "\n");
    if(simpledu_log_write(buff, strlen(buff)) == -1) return EXIT_FAILURE;
    return EXIT_SUCCESS;
}

int simpledu_log_RECV_SIGNAL(int sig){
    if(filedes == -1) return EXIT_SUCCESS;
    double t; if(simpledu_gettime(&t)) return EXIT_FAILURE; 
    char buff[MAXN]; if(sprintf(buff, "%.2f %08d RECV_SIGNAL %d\n", t, getpid(), sig) < 0) return EXIT_FAILURE;
    if(simpledu_log_write(buff, strlen(buff)) == -1) return EXIT_FAILURE;
    return EXIT_SUCCESS;
}

int simpledu_log_ENTRY(long long unsigned size, const char *path){
    if(filedes == -1) return EXIT_SUCCESS;
    double t; if(simpledu_gettime(&t)) return EXIT_FAILURE; 
    char buff[MAXN]; if(sprintf(buff, "%.2f %08d ENTRY %llu %s\n", t, getpid(), size, path) < 0) return EXIT_FAILURE;
    if(simpledu_log_write(buff, strlen(buff)) == -1) return EXIT_FAILURE;
    return EXIT_SUCCESS;
}

void simpledu_exit(int status){
    int res = simpledu_log_EXIT(status);
    if(res) exit(res);
    exit(status);
}

int simpledu_kill(pid_t pid, int sig){
    int res = simpledu_log_SEND_SIGNAL(sig, pid);
    if(res) return res;
    return kill(pid, sig);
}
