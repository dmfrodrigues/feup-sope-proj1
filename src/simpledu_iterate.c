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


int simpledu_process(const char* simpledu_path, int filedes[2], struct dirent *dir_point, const char *path, off_t *size, simpledu_args_t arg, char *envp[]) {
    if (strcmp(dir_point->d_name, PARENT_DIR) == 0 || strcmp(dir_point->d_name, THIS_DIR  ) == 0) return 10; // ????
    
    // New path
    char new_path[PATH_MAX]; strcpy(new_path, path);
    // printf("New path 1: %s\n", new_path);
    if(simpledu_join_path(new_path, dir_point->d_name)) return EXIT_FAILURE;
    // printf("New path 2: %s\n", new_path);
    // New mode
    simpledu_mode_t new_mode;
    if (simpledu_mode(new_path, &new_mode)) return EXIT_FAILURE;
    
    if (new_mode == simpledu_mode_dir) {
        // printf("Dir\n");
        int pid = fork();
        int status;

        if (pid > 0) {  // parent
            waitpid(-1, &status, 0);
            if(status) return EXIT_FAILURE;
        } else if (pid == 0) {  // child
            --arg.max_depth;
            close(filedes[0]);
            arg.pipe_filedes = filedes[1];

            if (simpledu_args_set_files(&arg, 1, new_path))
                return EXIT_FAILURE;

            char **new_argv = NULL;
            if (simpledu_args_toargv(&arg, &new_argv)) return EXIT_FAILURE;
            if (execve(simpledu_path, new_argv, envp)) return EXIT_FAILURE;
        }
    }
    else if (new_mode == simpledu_mode_lnk) {
        // printf("Symb Link\n");
        if (arg.dereference) {  // if -L

            /*
            // printf("Dereferencing\n");
            char resolved_path[PATH_MAX];
            if (realpath(new_path, resolved_path) == NULL) return EXIT_FAILURE;
            
            printf("New      Path: %s\n", new_path);
            printf("Resolved Path: %s\n", resolved_path);
            strcpy(resolved_path, new_path);
            
            // - check if dir
            // - if dir do what?
            // - replace dir_point (to change dir_point->d_name)

            simpledu_process(simpledu_path, filedes, dir_point, resolved_path, size, arg, envp);
            */
        }

        else {  // -L was not passed as argument. Process as if it was a regular file.

            off_t file_size = simpledu_stat(new_path, arg.apparent_size);
            if (file_size == -1) return EXIT_FAILURE;

            *size += file_size;

            if (arg.max_depth > 0 && arg.all) {
                printf("%ld\t%s\n", simpledu_block(file_size, arg.block_size), new_path);
            }
        }
    }

    else if (new_mode == simpledu_mode_reg) {
        // printf("Reg\n");
        off_t file_size = simpledu_stat(new_path, arg.apparent_size);
        if (file_size == -1) return EXIT_FAILURE;

        *size += file_size;

        if (arg.max_depth > 0 && arg.all)
            printf("%ld\t%s\n", simpledu_block(file_size, arg.block_size), new_path);
    }

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
    if (simpledu_mode(path, &mode)){
        fprintf(stderr, "du: cannot access '%s': No such file or directory\n", path);
        return EXIT_FAILURE;
    }
    off_t folder_size = simpledu_stat(path, arg.apparent_size);
    if(folder_size == -1) return EXIT_FAILURE;
    *size += folder_size;
    switch (mode) {
        case simpledu_mode_dir:{
            DIR *dir_to_iter = opendir(path);
            if (dir_to_iter == NULL){
                switch(errno){
                    case EACCES:
                        fprintf(stderr, "du: cannot read directory '%s': Permission denied\n", path);
                        break;
                    default:
                        break;
                }
                close(filedes[0]);
                close(filedes[1]);
                return EXIT_FAILURE;
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
                if (simpledu_mode(new_path, &new_mode)) return EXIT_FAILURE;
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
                            // printf("\nResolved Path: %s\n", new_path);
                            strcpy(resolved_path, new_path);
                            DIR * error_open;
                            if ( (error_open = opendir(resolved_path)) != NULL) {
                                struct dirent *dir_point_2 = NULL;
                                dir_point_2 = readdir(error_open);
                                // printf("\nName: %s", dir_point_2->d_name);
                                simpledu_join_path(new_path, dir_point_2->d_name);
                                off_t symb_link_file_size_2 = simpledu_stat(new_path, arg.apparent_size);
                                
                                *size += symb_link_file_size_2;
                                // pritnf("\n%ld", symb_link_file_size_2);
                                // printf("\nHere\n");
                                printf("%ld\t%s\n", simpledu_block(symb_link_file_size_2, arg.block_size), new_path);
                                // *size += symb_link_file_size_2;

                            }

                            // printf("\nsymb link path:%s \n", resolved_path);
                            off_t symb_link_file_size = simpledu_stat_2(resolved_path, arg.apparent_size);
                            // printf("\nsymb_link_file_size: %ld\n", symb_link_file_size);
                            if (symb_link_file_size == -1) return EXIT_FAILURE;

                            

                            if (arg.max_depth > 0 && arg.all) {
                                // printf("\nI'm here\n");
                                printf("%ld\t%s\n", simpledu_block(*size, arg.block_size), resolved_path);
                            }
                            
                            *size += symb_link_file_size;

                        } else {  // -L was not passed as argument. Process as if it
                                // was a regular file.
                            // display size and relative path/(name)
                            // printf("\nups.I'm not regular.\n");
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

            break;
        }
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
