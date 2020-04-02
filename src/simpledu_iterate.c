#include "simpledu_iterate.h"

#include <dirent.h>
#include <stdio.h>
#include <string.h>
#include <sys/wait.h>
#include <errno.h>

#include "simpledu_stat.h"

static const char PARENT_DIR[] = "..";
static const char THIS_DIR[] = ".";

int simpledu_get_program_path(char *path) {
    if (getcwd(path, PATH_MAX) == NULL) return EXIT_FAILURE;
    strcat(path, "/simpledu");
    return EXIT_SUCCESS;
}

int simpledu_iterate(const char *path, int *pipe_id, size_t *reads_pipe, off_t *size, simpledu_args_t arg, char *envp[]) {
    // Size
    *size = 0;
    // Pipe
    int filedes[2];
    if(pipe(filedes)) return EXIT_FAILURE;
    *pipe_id = filedes[0];
    *reads_pipe = 0;
    // Get path
    char simpledu_path[PATH_MAX];
    if (simpledu_get_program_path(simpledu_path)) return EXIT_FAILURE;
    // Mode
    simpledu_mode_t mode;
    if (simpledu_mode(path, &mode)) return EXIT_FAILURE;
    switch (mode) {
        case simpledu_mode_dir:{
            DIR *dir_to_iter = opendir(path);
            if (dir_to_iter == NULL) return EXIT_FAILURE;
            // Iterating over the items of a directory
            struct dirent *dir_point = NULL;
            while ((dir_point = readdir(dir_to_iter)) != NULL) {
                if (strcmp(dir_point->d_name, PARENT_DIR) == 0 ||
                    strcmp(dir_point->d_name, THIS_DIR  ) == 0)
                    continue;
                //New path
                char new_path[PATH_MAX];
                strcat(strcat(strcpy(new_path, path), "/"), dir_point->d_name);
                // New mode
                simpledu_mode_t new_mode;
                if (simpledu_mode(new_path, &new_mode)) return EXIT_FAILURE;
                switch(new_mode){
                    case simpledu_mode_dir: {
                        ++(*reads_pipe);
                        int pid = fork();
                        int status;

                        if (pid > 0) {  // parent
                            waitpid(-1, &status, 0);
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
                            char symb_link_buf[PATH_MAX];
                            ssize_t len = readlink(dir_point->d_name, symb_link_buf,
                                                sizeof(symb_link_buf) - 1);

                            // if no error occured
                            if (len != -1) symb_link_buf[len] = '\0';

                            // symb_link_buf -> Path of symb link relative to the
                            // directory. Examples:
                            //      ../testing_2/testing_symb_link_2_pls_work.txt
                            //      ./testtt/to_symb_link.txt

                            off_t symb_link_file_size = simpledu_stat(
                                symb_link_buf, arg.apparent_size, arg.block_size);
                            if (symb_link_file_size == -1) return EXIT_FAILURE;

                            *size += symb_link_file_size;
                            // If it needs to display file size
                            if (arg.max_depth > 0 && arg.all) {
                                printf("%ld\t%s\n", symb_link_file_size,
                                    symb_link_buf);
                            }
                        } else {  // -L was not passed as argument. Process as if it
                                // was a regular file.
                            // display size and relative path/(name)
                            off_t file_size = simpledu_stat(
                                new_path, arg.apparent_size, arg.block_size);
                            if (file_size == -1) return EXIT_FAILURE;
                            *size += file_size;
                            // If it needs to display file size
                            if (arg.max_depth > 0 && arg.all) {
                                printf("%ld\t%s\n", file_size, new_path);
                            }
                        }
                    } break;
                    case simpledu_mode_reg: {
                        off_t file_size = simpledu_stat(new_path, arg.apparent_size,
                                                    arg.block_size);
                        if (file_size == -1) return EXIT_FAILURE;
                        *size += file_size;
                        if (arg.max_depth > 0 && arg.all)
                            printf("%lld\t%s\n", (long long)file_size, new_path);
                    } break;
                    default: break;
                }
            }

            // After iterating over directory, will try to display results
            *size += simpledu_stat(path, arg.apparent_size, arg.block_size);
            closedir(dir_to_iter);

            break;
        }
        default: return EXIT_FAILURE;
    }

    close(filedes[1]);

    return EXIT_SUCCESS;
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

int simpledu_retrieve(int pipe_filedes, size_t reads_pipe, off_t *size) {
    *size = 0;
    if(pipe_filedes == -1) return EXIT_SUCCESS;
    //Repeat until an error exit code is returned
    char line[PIPE_BUF];
    /*
    int r, ret;
    while((r = waitpid(-1, &ret, 0)) >= 0){
        //If a child has returned
        if(r > 0){
            if(ret) return EXIT_FAILURE;
            if(readline(pipe_filedes, line)) return EXIT_FAILURE;
            off_t more_size;
            if(sscanf(line, "%ld", &more_size)!=1) return EXIT_FAILURE;
            *size += more_size;
        }
    }
    if(errno != ECHILD) return EXIT_FAILURE;
     */
    for(size_t i = 0; i < reads_pipe; ++i){
        if(readline(pipe_filedes, line)) return EXIT_FAILURE;
        off_t more_size;
        if(sscanf(line, "%ld", &more_size)!=1) return EXIT_FAILURE;
        *size += more_size;
    }
    return EXIT_SUCCESS;
}

int simpledu_print(const char *path, off_t size, off_t more_size,
                   simpledu_args_t arg) {
    off_t total_size = size + (arg.separate_dirs ? 0 : more_size);
    if (arg.max_depth >= 0) {
        printf("%ld\t%s\n", total_size, path);
    }
    if(arg.pipe_filedes != -1){
        char buf[PIPE_BUF];
        sprintf(buf, "%ld\n", size+more_size);
        write(arg.pipe_filedes, buf, strlen(buf));
    }
    return EXIT_SUCCESS;
}
