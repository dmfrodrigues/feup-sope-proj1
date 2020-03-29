#include "unnamed_functions.h"
#include "simpledu_stat.h"
#include "simpledu_log.h"

#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <limits.h>
#include <unistd.h>



int simpledu_iterate(const char *path, int *pipe_pid, simpledu_args_t arg, char *envp[]) {

    //Sum of all file sizes in directory iteration
    //This number will be sent to father process' pipe
    long long result = 0;


    // check if the path given is a directory
    if (!simpledu_dir(path)) return -1;

    DIR *dir_to_iter;

    struct dirent *dir_point;

    if ( (dir_to_iter = opendir(path) ) == NULL) {
        perror("Could not open directory");
        return EXIT_FAILURE;
    }


    // if nothing to show printf "." ?

    // Iterating over the items of a directory
    while ( (dir_point = readdir(dir_to_iter)) != NULL) {

        // if dir
        if (simpledu_dir(dir_point->d_name)) {
            
            //Must deal with dir_to_iter being open before doing fork (dont know how yet)

            int pid = fork();
            int status;

            if (pid > 0) { //parent
                //Will receive results from pipe here
                waitpid(pid, &status, 0);

                //Display results from children here

            } else if (pid == 0) { //child
                char new_path[PATH_MAX];
                strcpy(new_path, dir_point->d_name);
                strcat(new_path, "/simpledu");  

                if (arg.max_depth >= 0) --arg.max_depth;
                char **new_argv = NULL;
                if (simpledu_args_toargv(&arg, &new_argv)) simpledu_exit(EXIT_FAILURE);
                if (execve(new_path, new_argv, envp)) simpledu_exit(EXIT_FAILURE);

            } else {
                // Should it exit with this?
                simpledu_exit(EXIT_FAILURE);
            }
        }

        // if symbolic link
        else if (simpledu_symb_link(dir_point->d_name)) {
            
            if (arg.dereference) {
                // to dereference
                // placeholder code
                char symb_link_buf[PATH_MAX];
                ssize_t len = readlink(dir_point->d_name, symb_link_buf, sizeof(symb_link_buf)-1);
                
                // if no error occured
                if (len != -1)
                    symb_link_buf[len] = '\0';

                // symb_link_buf -> Path of symb link

                // display it
                
            }

            else {
                // as if it was a regular file (copy from the corresponding if when that is complete)
            }
        }

        // if regular file
        // TODO dont think the size or the formatting correspond to those of the du command...
        // still don't know how to fix it -_-
        else if (simpledu_reg_file(dir_point->d_name)) {
            // display size and relative path/(name)
            // Placeholder code. Make it display full relative path from "starting" directory
            off_t file_size = simpledu_stat(dir_point->d_name, arg.block_size);

            //If it needs to display file size
            if (arg.max_depth > 0 && arg.all){
                printf("%lld\t%s\n", (long long) file_size,  dir_point->d_name);
            }
        }

    }

    //After iterating over directory, will try to display results
    if (arg.max_depth >= 0){
        printf("%lld\t%s\n", result,  path);
    }
    
    closedir(dir_to_iter);
    return EXIT_SUCCESS;
}
