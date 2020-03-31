#include "unnamed_functions.h"

#include <limits.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "simpledu_log.h"
#include "simpledu_stat.h"
#define __USE_GNU
#include <errno.h>
#include <unistd.h>

static const char PARENT_DIR[] = "..";
static const char THIS_DIR[] = ".";

int simpledu_iterate(int *pipe_pid, simpledu_args_t arg, char *envp[]) {
    // Get path
    char simpledu_path[PATH_MAX];{
        if (getcwd(simpledu_path, PATH_MAX) == NULL)
            simpledu_exit(EXIT_FAILURE);
        strcat(simpledu_path, "/simpledu");
    }
    // Iterate
    for (size_t i = 0; i < arg.filesc; ++i) {
        off_t result = 0;
            
        char *path = arg.files[i];
        if (!simpledu_dir(path)) return EXIT_FAILURE;

        DIR *dir_to_iter = opendir(path);
        if (dir_to_iter == NULL) return EXIT_FAILURE;

        // Iterating over the items of a directory
        struct dirent *dir_point = NULL;
        while ((dir_point = readdir(dir_to_iter)) != NULL) {

            if(strcmp(dir_point->d_name, PARENT_DIR) == 0 ||
               strcmp(dir_point->d_name, THIS_DIR  ) == 0) continue;

            char new_path[PATH_MAX];
            strcat(strcat(strcpy(new_path, path), "/"), dir_point->d_name);

            //printf("EXPLORING %s\n", new_path);
            // if dir
            if (simpledu_dir(new_path)) { //printf("  IS A DIR\n");

                int pid = fork();
                int status;

                if (pid > 0) {  // parent
                    // Will receive results from pipe here
                    waitpid(pid, &status, 0);
                    // printf("Child %d died\n", pid);

                    // Display results from children here
                } else if (pid == 0) {  // child
                    simpledu_args_t new_arg;
                    if(simpledu_args_copy(&new_arg, &arg)) return EXIT_FAILURE;
                    --new_arg.max_depth;

                    if(simpledu_args_set_files(&new_arg, 1, new_path)) return EXIT_FAILURE;

                    char **new_argv = NULL;
                    if (simpledu_args_toargv(&new_arg, &new_argv)) return EXIT_FAILURE;
                    if (execve(simpledu_path, new_argv, envp)) return EXIT_FAILURE;
                }
            } /*else if (simpledu_symb_link(new_path)) {
                if (arg.dereference) {
                    // to dereference
                    // placeholder code
                    char symb_link_buf[PATH_MAX];
                    ssize_t len = readlink(dir_point->d_name, symb_link_buf,
                                           sizeof(symb_link_buf) - 1);

                    // if no error occured
                    if (len != -1) symb_link_buf[len] = '\0';

                    // symb_link_buf -> Path of symb link

                    // display it
                }

                else {
                    // as if it was a regular file (copy from the corresponding
                    // if when that is complete)
                }
            }*/
            else if (simpledu_reg_file(new_path)) { //printf("  IS A REG FILE\n");

                // display size and relative path/(name)
                // Placeholder code. Make it display full relative path from "starting" directory 
                off_t file_size = simpledu_stat(new_path, arg.apparent_size, arg.block_size);
                if(file_size == -1) return EXIT_FAILURE;
                result += file_size;
                //If it needs to display file size
                if (arg.max_depth >= 0 && arg.all){
                    printf("%lld\t%s\n", (long long) file_size,  new_path);
                }
            }
        }

        // After iterating over directory, will try to display results
        //if (arg.max_depth >= 0) {
        printf("%ld\t%s\n", result + simpledu_stat(path, arg.apparent_size, arg.block_size), path);
        //}

        closedir(dir_to_iter);
    }

    return EXIT_SUCCESS;
}
