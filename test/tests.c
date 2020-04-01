#include "acutest.h"

#include "simpledu_args.h"
#include "simpledu_stat.h"

void test_simpledu_args_ctor1(void) {
    int argc = 1;
    const char *argv_const[2] = {"prog", NULL};
    char *argv[2];
    argv[argc] = NULL;
    for (int i = 0; i < argc; ++i) {
        argv[i] = malloc((strlen(argv_const[i]) + 1)*sizeof(char));
        strcpy(argv[i], argv_const[i]);
    }
    simpledu_args_t args;
    TEST_CHECK(simpledu_args_ctor(&args, argc, argv) == EXIT_SUCCESS);
    TEST_CHECK(args.all == false);
    TEST_CHECK(args.block_size == 1024);
    TEST_CHECK(args.count_links == false);
    TEST_CHECK(args.dereference == false);
    TEST_CHECK(args.separate_dirs == false);
    TEST_CHECK(args.max_depth >= PATH_MAX / 2);
    TEST_CHECK(args.filesc == 1);
    TEST_CHECK(strcmp(args.files[0], ".") == 0);
    TEST_CHECK(simpledu_args_dtor(&args) == EXIT_SUCCESS);
    for (int i = 0; i < argc; ++i){ free(argv[i]); argv[i] = NULL; }
}

void test_simpledu_args_ctor2(void){
    int argc = 7;
    const char *argv_const[8] = {"prog", "-a",  "-l", "-S", "--blocksize=300",
                                 "./a",  "./b", NULL};
    char *argv[8];
    argv[argc] = NULL;
    for (int i = 0; i < argc; ++i) {
        argv[i] = malloc((strlen(argv_const[i]) + 1)*sizeof(char));
        strcpy(argv[i], argv_const[i]);
    }
    simpledu_args_t args;
    TEST_CHECK(simpledu_args_ctor(&args, argc, argv) == EXIT_SUCCESS);
    TEST_CHECK(args.all == true);
    TEST_CHECK(args.block_size == 300);
    TEST_CHECK(args.count_links == true);
    TEST_CHECK(args.dereference == false);
    TEST_CHECK(args.separate_dirs == true);
    TEST_CHECK(args.max_depth >= PATH_MAX / 2);
    TEST_CHECK(args.filesc == 2);
    TEST_CHECK(strcmp(args.files[0], "./a") == 0);
    TEST_CHECK(strcmp(args.files[1], "./b") == 0);
    TEST_CHECK(simpledu_args_dtor(&args) == EXIT_SUCCESS);
    for (int i = 0; i < argc; ++i){ free(argv[i]); argv[i] = NULL; }
}

void test_simpledu_args_ctor3(void){
    int argc = 7;
    const char *argv_const[8] = {"prog", "-a",  "-l",  "-S",
                                 "-B",   "./a", "./b", NULL};
    char *argv[8];
    argv[argc] = NULL;
    for (int i = 0; i < argc; ++i) {
        argv[i] = malloc((strlen(argv_const[i]) + 1)*sizeof(char));
        strcpy(argv[i], argv_const[i]);
    }
    simpledu_args_t args;
    TEST_CHECK(simpledu_args_ctor(&args, argc, argv) == EXIT_FAILURE);
    for (int i = 0; i < argc; ++i) free(argv[i]);
}

void test_simpledu_args_equal(void){
    int argc = 8;
    const char *argv_const[9] = {"prog", "-a",  "-l",  "-S", "-B",
                                    "300",  "./a", "./b", NULL};
    char *argv[9];
    argv[argc] = NULL;
    for (int i = 0; i < argc; ++i) {
        argv[i] = malloc((strlen(argv_const[i]) + 1)*sizeof(char));
        strcpy(argv[i], argv_const[i]);
    }
    simpledu_args_t a1;
    TEST_CHECK(simpledu_args_ctor(&a1, argc, argv) == EXIT_SUCCESS);
    simpledu_args_t a2;
    TEST_CHECK(simpledu_args_ctor(&a2, argc, argv) == EXIT_SUCCESS);
    TEST_CHECK(simpledu_args_equal(&a1, &a2));
    const char *argv2_const[9] = {"prog", "-a",  "-l",  "-S", "-B",
                                    "299",  "./a", "./b", NULL};
    char *argv2[9];
    argv2[argc] = NULL;
    for (int i = 0; i < argc; ++i) {
        argv2[i] = malloc((strlen(argv2_const[i]) + 1)*sizeof(char));
        strcpy(argv2[i], argv2_const[i]);
    }
    TEST_CHECK(simpledu_args_ctor(&a2, argc, argv2) == EXIT_SUCCESS);
    TEST_CHECK(!simpledu_args_equal(&a1, &a2));
    for (int i = 0; i < argc; ++i) free(argv[i]);
    for (int i = 0; i < argc; ++i) free(argv2[i]);
    TEST_CHECK(simpledu_args_dtor(&a1) == EXIT_SUCCESS);
    TEST_CHECK(simpledu_args_dtor(&a2) == EXIT_SUCCESS);
}

void test_simpledu_args_copy(void){
    int argc = 8;
    const char *argv_const[9] = {"prog", "-a",  "-l",  "-S", "-B",
                                    "300",  "./a", "./b", NULL};
    char *argv[9];
    argv[argc] = NULL;
    for (int i = 0; i < argc; ++i) {
        argv[i] = malloc((strlen(argv_const[i]) + 1)*sizeof(char));
        strcpy(argv[i], argv_const[i]);
    }
    simpledu_args_t a1;
    TEST_CHECK(simpledu_args_ctor(&a1, argc, argv) == EXIT_SUCCESS);
    simpledu_args_t a2;
    TEST_CHECK(simpledu_args_ctor(&a2, 0, NULL) == EXIT_SUCCESS);
    TEST_CHECK(!simpledu_args_equal(&a1, &a2));
    TEST_CHECK(simpledu_args_copy(&a2, &a1) == EXIT_SUCCESS);
    TEST_CHECK(simpledu_args_equal(&a1, &a2));

    TEST_CHECK(simpledu_args_dtor(&a1) == EXIT_SUCCESS);
    TEST_CHECK(simpledu_args_dtor(&a2) == EXIT_SUCCESS);
}

void test_simpledu_args_set_files(void){
    int argc = 8;
    const char *argv_const[9] = {"prog", "-a",  "-l",  "-S", "-B",
                                    "300",  "./a", "./b", NULL};
    char *argv[9];
    argv[argc] = NULL;
    for (int i = 0; i < argc; ++i) {
        argv[i] = malloc((strlen(argv_const[i]) + 1)*sizeof(char));
        strcpy(argv[i], argv_const[i]);
    }
    simpledu_args_t arg;
    TEST_CHECK(simpledu_args_ctor(&arg, argc, argv) == EXIT_SUCCESS);

    TEST_CHECK(simpledu_args_set_files(&arg, 3, "./c", "./d", "./e") == EXIT_SUCCESS);
    TEST_CHECK(arg.filesc == 3);
    TEST_CHECK(strcmp(arg.files[0], "./c") == 0);
    TEST_CHECK(strcmp(arg.files[1], "./d") == 0);
    TEST_CHECK(strcmp(arg.files[2], "./e") == 0);   

    TEST_CHECK(simpledu_args_dtor(&arg) == EXIT_SUCCESS);
}

void test_simpledu_symb_link(void){
    TEST_CHECK(simpledu_symb_link   ("test"                             ) == false);
    TEST_CHECK(simpledu_symb_link   ("test/tests.c"                     ) == false);
    TEST_CHECK(simpledu_symb_link   ("test/test-folder/D1/Db/Dx/symlnk" ) == true );
}

void test_simpledu_dir(void){
    TEST_CHECK(simpledu_dir         ("test"                             ) == true );
    TEST_CHECK(simpledu_dir         ("test/tests.c"                     ) == false);
    TEST_CHECK(simpledu_dir         ("test/test-folder/D1/Db/Dx/symlnk" ) == false);
}

TEST_LIST = {
    {"simpledu_args_ctor1"      , test_simpledu_args_ctor1      },
    {"simpledu_args_ctor2"      , test_simpledu_args_ctor2      },
    {"simpledu_args_ctor3"      , test_simpledu_args_ctor3      },
    {"simpledu_args_equal"      , test_simpledu_args_equal      },
    {"simpledu_args_copy"       , test_simpledu_args_copy       },
    {"simpledu_args_set_files"  , test_simpledu_args_set_files  },
    {"simpledu_symb_link"       , test_simpledu_symb_link       },
    {"simpledu_dir"             , test_simpledu_dir             },
    {NULL, NULL}
};
