#include "unnamed_functions.h"
#include "simpledu_stat.h"


int simpledu_iterate(const char *path, int *pipe_pid) {

    // check if the path given is a directory
    if (!simpledu_dir(path)) return -1;

    DIR *dir_to_iter;

    struct dirent *dir_point;

    if ( (dir_to_iter = opendir(path) ) == NULL) {
        perror("Could not open directory");
        return EXIT_FAILURE;
    }

    // Iterating over the items of a directory
    while ( (dir_point = readdir(dir_to_iter)) != NULL) {
        
        // if dir
        if (simpledu_dir(dir_point->d_name)) {
            // process it
            /*

            */
        }

        // if symbolic link
        else if (simpledu_symb_link(dir_point->d_name)) {


        }

        // if regular file
        else if (simpledu_reg_file(dir_point->d_name)) {
            // display relative path/(name)
            // display size

        }

    }
    

    return EXIT_SUCCESS;
}
