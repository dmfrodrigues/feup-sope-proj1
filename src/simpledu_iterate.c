#include "simpledu_iterate.h"

#include <dirent.h>
#include <stdio.h>
#include <string.h>
#include <sys/wait.h>
#include <errno.h>
#include <libgen.h>
#include <stdlib.h>

#include "simpledu_stat.h"
#include "simpledu_args.h"
#include "simpledu_signals.h"
#include "simpledu_log.h"

#define BUFFER_SIZE 1024+PATH_MAX

static const char PARENT_DIR[] = "..";
static const char THIS_DIR[] = ".";

int simpledu_join_path(char *dest, const char *src){
    size_t N = strlen(dest);
    if(dest[N-1] != '/') strcat(dest, "/");
    strcat(dest, src);
    return EXIT_SUCCESS;
}

int simpledu_get_program_path(char *path, size_t n) {
    if(readlink("/proc/self/exe", path, n) == -1) return EXIT_FAILURE;
    return EXIT_SUCCESS;
}


int simpledu_iterate(const char *path, int *pipe_id, off_t *size, simpledu_args_t arg, char *envp[]) {
    sleep(1);

    int ret = EXIT_SUCCESS;

    char buf[BUFFER_SIZE];
    // Size
    *size = 0;

    // Pipe
    int filedes[2];
    if(pipe(filedes)) return EXIT_FAILURE;
    *pipe_id = filedes[0];

    // Get path, actual path
    char simpledu_path[PATH_MAX];{
        if (simpledu_get_program_path(simpledu_path, PATH_MAX - 1)) return EXIT_FAILURE;
    }
    char path_actual[PATH_MAX];{
        if(arg.dereference){
            if (realpath(path, path_actual) == NULL){
                sprintf(buf, "du: cannot access '%s': %s\n", path, strerror(errno));
                write(STDERR_FILENO, buf, strlen(buf));
                return 2;
            }
        } else {
            strcpy(path_actual, path);
        }
    }
    // Mode
    simpledu_mode_t mode;{
        if (simpledu_mode(path_actual, &mode)) {
            sprintf(buf, "du: cannot access '%s': %s\n", path, strerror(errno));
            write(STDERR_FILENO, buf, strlen(buf));
            return 2;
        }
    }
    off_t folder_size = simpledu_stat(path_actual, arg.apparent_size);
    if (folder_size == -1) return EXIT_FAILURE;
    
    *size += folder_size;
    switch (mode) {
        case simpledu_mode_dir:{
            DIR *dir_to_iter = opendir(path);
            if (dir_to_iter == NULL) {
                sprintf(buf, "du: cannot read directory '%s': %s\n", path, strerror(errno));
                if(write(STDERR_FILENO, buf, strlen(buf)) != (ssize_t)strlen(buf)) return EXIT_FAILURE;
                close(filedes[0]);
                close(filedes[1]);
                return EXIT_FAILURE;
            }
            // Iterating over the items of a directory
            struct dirent *dir_point = NULL;
            while ((dir_point = readdir(dir_to_iter)) != NULL) {
                // Ignore parent and this directory
                if (strcmp(dir_point->d_name, PARENT_DIR) == 0 ||
                    strcmp(dir_point->d_name, THIS_DIR  ) == 0)
                    continue;
                // New path, resolved path
                char new_path[PATH_MAX];{
                    strcpy(new_path, path);
                    if (simpledu_join_path(new_path, dir_point->d_name))
                        return EXIT_FAILURE;
                }
                char new_path_actual[PATH_MAX];{
                    if (arg.dereference) {
                        if (realpath(new_path, new_path_actual) == NULL) {
                            sprintf(buf, "du: cannot access '%s'\n", new_path);
                            if(write(STDERR_FILENO, buf, strlen(buf)) != (ssize_t)strlen(buf)) return EXIT_FAILURE;
                            ret = EXIT_FAILURE;
                            continue;
                        }
                    } else {
                        strcpy(new_path_actual, new_path);
                    }
                }
                // New mode
                simpledu_mode_t new_mode;{
                    if(simpledu_mode(new_path_actual, &new_mode)){
                        sprintf(buf, "du: cannot access '%s'\n", new_path);
                        if(write(STDERR_FILENO, buf, strlen(buf)) != (ssize_t)strlen(buf)) return EXIT_FAILURE;
                        ret = EXIT_FAILURE;
                        continue;
                    }
                }
                switch (new_mode) {
                    case simpledu_mode_dir: {
                        pid_t pid = fork();

                        if (pid > 0) {  // parent
                            if(arg.children_process_group == 0){ 
                                //Setting the arg for first parent
                                arg.children_process_group = pid;
                                initialize_data(arg.children_process_group);
                            }
                        } else if (pid == 0) {  // child
                            if(arg.children_process_group == 0){
                                //Setting the arg and pgid for first child
                                arg.children_process_group = getpid();
                                setpgid(0, getpid());
                            }
                            else {
                                //Setting the pgid for all other children (arg will be inherited)
                                //Only for remainding children of first process
                                setpgid(0, arg.children_process_group);
                            }
                            --arg.max_depth;
                            close(filedes[0]);
                            arg.pipe_filedes = filedes[1];

                            if (simpledu_args_set_files(&arg, 1, new_path))
                                return EXIT_FAILURE;

                            char **new_argv = NULL;
                            if (simpledu_args_toargv(&arg, &new_argv))
                                return EXIT_FAILURE;
                            if (execve(simpledu_path, new_argv, envp))
                                return EXIT_FAILURE;
                        }
                    } break;
                    case simpledu_mode_lnk:
                    case simpledu_mode_reg: {
                        off_t file_size = simpledu_stat(new_path, arg.apparent_size);
                        if (file_size == -1) return EXIT_FAILURE;
                        *size += file_size;
                        if (arg.max_depth > 0 && arg.all) {
                            sprintf(buf, "%ld\t%s\n", simpledu_block(file_size, arg.block_size), new_path);
                            simpledu_log_ENTRY(simpledu_block(file_size, arg.block_size), new_path);
                            if(write(STDOUT_FILENO, buf, strlen(buf)) != (ssize_t)strlen(buf)) return EXIT_FAILURE;
                        }
                    } break;
                    default: break;
                }
                
            } 

            closedir(dir_to_iter);

        } break;
        case simpledu_mode_reg:
        case simpledu_mode_lnk:
            break;
        default:
            return EXIT_FAILURE;
    }

    close(filedes[1]);

    return ret;
}

int readline(int fd, char *str) {
    ssize_t n;
    do {
        n = read(fd, str, 1);
    } while (n > 0 && *str++ != '\n');
    *(--str) = '\0';
    if(n <= 0){
        errno = EIO;
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}

int simpledu_retrieve(int pipe_filedes, off_t *size) {
    int ret = EXIT_SUCCESS;
    *size = 0;
    if(pipe_filedes == -1) return EXIT_SUCCESS;
    //Repeat until an error exit code is returned
    char line[PIPE_BUF];
    
    int r, status;
    while((r = waitpid(-1, &status, 0)) >= 0){
        //If a child has returned
        if(r > 0){
            if(status) ret = EXIT_FAILURE;
            if(readline(pipe_filedes, line)) return 2;
            off_t more_size;
            if(sscanf(line, "%ld", &more_size)!=1) return 2;
            *size += more_size;
        }
    }
    if (errno != ECHILD){
        return 2;
    }
    return ret;
}

int simpledu_print(const char *path, off_t size, off_t more_size, simpledu_args_t arg) {
    off_t total_size = size + (arg.separate_dirs ? 0 : more_size);
    if (arg.max_depth >= 0) {
        char buf[BUFFER_SIZE];
        sprintf(buf, "%ld\t%s\n", simpledu_block(total_size, arg.block_size), path);
        simpledu_log_ENTRY(simpledu_block(total_size, arg.block_size), path);
        if(write(STDOUT_FILENO, buf, strlen(buf)) != (ssize_t)strlen(buf)) return EXIT_FAILURE;
    }
    if(arg.pipe_filedes != -1){
        char buf[PIPE_BUF];
        sprintf(buf, "%ld\n", size+more_size);
        if(write(arg.pipe_filedes, buf, strlen(buf)) != (ssize_t)strlen(buf)) return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}