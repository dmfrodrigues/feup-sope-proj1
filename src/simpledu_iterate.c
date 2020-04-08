#include "simpledu_iterate.h"

#include <dirent.h>
#include <stdio.h>
#include <string.h>
#include <sys/wait.h>
#include <errno.h>
#include <libgen.h>
#include <stdlib.h>

#include "simpledu_stat.h"

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
    int ret = EXIT_SUCCESS;
    // Size
    *size = 0;

    // Pipe
    int filedes[2];
    if(pipe(filedes)) return EXIT_FAILURE;
    *pipe_id = filedes[0];

    // Get path
    char simpledu_path[PATH_MAX];
    if (simpledu_get_program_path(simpledu_path, PATH_MAX-1)) return EXIT_FAILURE;

    // Mode
    simpledu_mode_t mode;
    char resolved_path[PATH_MAX];
    if (arg.dereference) {
        if (realpath(path, resolved_path) == NULL){
            fprintf(stderr, "du: cannot access '%s': No such file or directory\n", path);
            return 2;
        }
        simpledu_mode(resolved_path, &mode);
    }
    else if (simpledu_mode(path, &mode)){
        fprintf(stderr, "du: cannot access '%s': No such file or directory\n", path);
        return 2;
    }
    off_t folder_size;
    if (arg.dereference) {
        folder_size = simpledu_stat(resolved_path, arg.apparent_size);
    }
    else {
        folder_size = simpledu_stat(path, arg.apparent_size);
    }
    if(folder_size == -1) return EXIT_FAILURE;
    *size += folder_size;
    switch (mode) {
        case simpledu_mode_dir:{
            DIR *dir_to_iter = opendir(path);
            if (dir_to_iter == NULL){
                switch(errno){
                    case EACCES:
                        fprintf(stderr, "du: cannot read directory '%s': Permission denied\n", path);
                        close(filedes[0]);
                        close(filedes[1]);
                        return EXIT_FAILURE;
                    default:
                        break;
                }
                close(filedes[0]);
                close(filedes[1]);
                return 2;
            }
            // Iterating over the items of a directory
            struct dirent *dir_point = NULL;
            while ((dir_point = readdir(dir_to_iter)) != NULL) {
                // simpledu_process(simpledu_path, filedes, dir_point, path, size, arg, envp);
                
                if (strcmp(dir_point->d_name, PARENT_DIR) == 0 ||
                    strcmp(dir_point->d_name, THIS_DIR  ) == 0)
                    continue;
                //New path
                char new_path[PATH_MAX]; strcpy(new_path, path);
                if(simpledu_join_path(new_path, dir_point->d_name)) return EXIT_FAILURE;
                // New mode
                simpledu_mode_t new_mode;
                char resolved_path_2[PATH_MAX];
                if (arg.dereference) {
                    if (realpath(new_path, resolved_path_2) == NULL){
                        fprintf(stderr, "du: cannot access '%s'\n", new_path);
                        ret = EXIT_FAILURE;
                        continue;
                    }
                    simpledu_mode(resolved_path_2, &new_mode);
                }
                else if (simpledu_mode(new_path, &new_mode)) return EXIT_FAILURE;
                switch(new_mode){
                    case simpledu_mode_dir: {
                        int pid = fork();

                        if (pid > 0) {  // parent

                        } else if (pid == 0) {  // child
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
                    case simpledu_mode_lnk: {

                        if (arg.dereference) {

                            char resolved_path[PATH_MAX];
                            if (realpath(new_path, resolved_path) == NULL) return EXIT_FAILURE;

                            int pid = fork();

                            if (pid > 0) {  // parent

                            } else if (pid == 0) {  // child
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

                        } else {  // -L was not passed as argument

                            off_t file_size = simpledu_stat(new_path, arg.apparent_size);
                            if (file_size == -1) return EXIT_FAILURE;
                            
                            // If it needs to display file size
                            if (arg.max_depth > 0 && arg.all) {
                                printf("%ld\t%s\n", simpledu_block(file_size, arg.block_size), new_path);
                            }
                            *size += file_size;
                        }
                    } break;
                    case simpledu_mode_reg: {
                        off_t file_size = simpledu_stat(new_path, arg.apparent_size);
                        if (file_size == -1) return EXIT_FAILURE;
                        *size += file_size;
                        if (arg.max_depth > 0 && arg.all)
                            printf("%ld\t%s\n", simpledu_block(file_size, arg.block_size), new_path);
                    } break;
                    default: break;
                }
                
            } 

            closedir(dir_to_iter);

        } break;
        case simpledu_mode_reg:{
        } break;
        case simpledu_mode_lnk:{
        } break;
        default: return EXIT_FAILURE;
    }

    close(filedes[1]);

    return ret;
}

int readline(int fd, char *str) {
    int n;
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
            if(status) ret = 2;
            if(readline(pipe_filedes, line)) return EXIT_FAILURE;
            off_t more_size;
            if(sscanf(line, "%ld", &more_size)!=1) return EXIT_FAILURE;
            *size += more_size;
        }
    }
    if(errno != ECHILD) return EXIT_FAILURE;
    return ret;
}

int simpledu_print(const char *path, off_t size, off_t more_size, simpledu_args_t arg) {
    off_t total_size = size + (arg.separate_dirs ? 0 : more_size);
    if (arg.max_depth >= 0) {
        printf("%ld\t%s\n", simpledu_block(total_size, arg.block_size), path);
    }
    if(arg.pipe_filedes != -1){
        char buf[PIPE_BUF];
        sprintf(buf, "%ld\n", size+more_size);
        if(write(arg.pipe_filedes, buf, strlen(buf)) != strlen(buf)) return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}