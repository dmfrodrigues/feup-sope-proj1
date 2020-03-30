#include "unnamed_functions.h"
#include "simpledu_stat.h"
#include "simpledu_log.h"

#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <limits.h>
#include <unistd.h>

static const char *BACK_DIRECTORY = "..";
static const char *CURRENT_DIRECTORY = ".";

int simpledu_iterate(int *pipe_pid, simpledu_args_t arg, char *envp[]) {

    //Sum of all file sizes in directory iteration
    //This number will be sent to father process' pipe
    long long result = 0;

    //Get path from environment variable PWD
    char *path = getenv("PWD");

    //If no files are given as arguments, will evaluate all files on directory by default
    bool evaluate_all_files = true;

    // check if the path given is a directory
    if (!simpledu_dir(path)) return EXIT_FAILURE;

    DIR *dir_to_iter;

    struct dirent *dir_point;

    if ( (dir_to_iter = opendir(path) ) == NULL) {
        perror("Could not open directory");
        return EXIT_FAILURE;
    }


    if (arg.filesc > 1){ //Will only evaluate the given files
        evaluate_all_files = false;
    }

    // Iterating over the items of a directory
    while ( (dir_point = readdir(dir_to_iter)) != NULL) {
        char new_path[PATH_MAX];
        //sprintf(new_path, "%s/%s", path, dir_point->d_name);

        //Dont know if this is needed, just in case
        if (strcmp(dir_point->d_name, BACK_DIRECTORY) == 0 || strcmp(dir_point->d_name, CURRENT_DIRECTORY) == 0) continue;

        printf("%s\n", new_path);

        //Shitty solution but should work 
        bool skip_this_file = true;

        if (!evaluate_all_files){
            for (size_t i = 0 ; i < arg.filesc ; ++i){
                if (arg.files[i] == dir_point->d_name){ //Found this file in the list
                    skip_this_file = false;
                    break;
                }
            }
            if (skip_this_file) break;
        }

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
                printf("Entered child %d\n", getpid());
                char **new_argv = NULL;
                simpledu_args_t new_arg = arg;

                //Updating arg's files (will only be '.')
                new_arg.filesc = 1;
                new_arg.files = malloc(new_arg.filesc * sizeof(char *));
                if (new_arg.files == NULL) return EXIT_FAILURE;
                const char *s = ".";
                char *str = malloc(2 * sizeof(char));
                if (str == NULL) {
                    free(new_arg.files);
                    return EXIT_FAILURE;
                }
                strcpy(str, s);
                new_arg.files[0] = str;

                //Making envp for new process
                char pwd[PATH_MAX];
                char pth[PATH_MAX];
                sprintf(pwd, "PWD=%s", new_path);
                sprintf(pth, "PATH=%s", getenv("PATH"));
                char *envList[] = {pwd, NULL}; 

                //Path to subdirectory
                strcat(new_path, "/simpledu");

                //Updating max_depth
                if (new_arg.max_depth >= 0) --new_arg.max_depth;

                if (simpledu_args_toargv(&new_arg, &new_argv)) return EXIT_FAILURE;
                if (execve(new_path, new_argv, envList)) {
                    printf("execve failed niggaa\n");
                    return EXIT_FAILURE;
                }

            } else {
                return EXIT_FAILURE;
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
            if (arg.max_depth >= 0 && arg.all){
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
