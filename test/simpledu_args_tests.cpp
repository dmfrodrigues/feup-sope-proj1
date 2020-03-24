#ifndef CATCH_CONFIG_MAIN
#define CATCH_CONFIG_MAIN
#endif //CATCH_CONFIG_MAIN
#include "catch.hpp"

extern "C" {
#include "simpledu_args.h"
}

TEST_CASE("simpledu_args_ctor1", "[simpledu_args_ctor1]") {
    int argc = 1;
    const char *argv_const[2] = {"prog", NULL};
    char *argv[2];
    argv[argc] = NULL;
    for (int i = 0; i < argc; ++i) {
        argv[i] = new char[strlen(argv_const[i]) + 1];
        strcpy(argv[i], argv_const[i]);
    }
    simpledu_args_t args;
    REQUIRE(simpledu_args_ctor(&args, argc, argv) == EXIT_SUCCESS);
    REQUIRE(args.all == false);
    REQUIRE(args.block_size == 1024);
    REQUIRE(args.count_links == false);
    REQUIRE(args.dereference == false);
    REQUIRE(args.separate_dirs == false);
    REQUIRE(args.max_depth >= PATH_MAX / 2);
    REQUIRE(args.filesc == 1);
    REQUIRE(strcmp(args.files[0], ".") == 0);
    REQUIRE(simpledu_args_dtor(&args) == EXIT_SUCCESS);
    for (int i = 0; i < argc; ++i) delete argv[i];
}

TEST_CASE("simpledu_args_ctor2", "[simpledu_args_ctor2]") {
    int argc = 7;
    const char *argv_const[8] = {"prog", "-a",  "-l", "-S", "--blocksize=300",
                                 "./a",  "./b", NULL};
    char *argv[8];
    argv[argc] = NULL;
    for (int i = 0; i < argc; ++i) {
        argv[i] = new char[strlen(argv_const[i]) + 1];
        strcpy(argv[i], argv_const[i]);
    }
    simpledu_args_t args;
    REQUIRE(simpledu_args_ctor(&args, argc, argv) == EXIT_SUCCESS);
    REQUIRE(args.all == true);
    REQUIRE(args.block_size == 300);
    REQUIRE(args.count_links == true);
    REQUIRE(args.dereference == false);
    REQUIRE(args.separate_dirs == true);
    REQUIRE(args.max_depth >= PATH_MAX / 2);
    REQUIRE(args.filesc == 2);
    REQUIRE(strcmp(args.files[0], "./a") == 0);
    REQUIRE(strcmp(args.files[1], "./b") == 0);
    REQUIRE(simpledu_args_dtor(&args) == EXIT_SUCCESS);
    for (int i = 0; i < argc; ++i) delete argv[i];
}

TEST_CASE("simpledu_args_ctor3", "[simpledu_args_ctor3]") {
    int argc = 7;
    const char *argv_const[8] = {"prog", "-a",  "-l",  "-S",
                                 "-B",   "./a", "./b", NULL};
    char *argv[8];
    argv[argc] = NULL;
    for (int i = 0; i < argc; ++i) {
        argv[i] = new char[strlen(argv_const[i]) + 1];
        strcpy(argv[i], argv_const[i]);
    }
    simpledu_args_t args;
    REQUIRE(simpledu_args_ctor(&args, argc, argv) == EXIT_FAILURE);
    for (int i = 0; i < argc; ++i) delete argv[i];
}

TEST_CASE("simpledu_args_equal", "[simpledu_args_equal]") {
    {
        int argc = 8;
        const char *argv_const[9] = {"prog", "-a",  "-l",  "-S", "-B",
                                     "300",  "./a", "./b", NULL};
        char *argv[9];
        argv[argc] = NULL;
        for (int i = 0; i < argc; ++i) {
            argv[i] = new char[strlen(argv_const[i]) + 1];
            strcpy(argv[i], argv_const[i]);
        }
        simpledu_args_t a1;
        REQUIRE(simpledu_args_ctor(&a1, argc, argv) == EXIT_SUCCESS);
        simpledu_args_t a2;
        REQUIRE(simpledu_args_ctor(&a2, argc, argv) == EXIT_SUCCESS);
        REQUIRE(simpledu_args_equal(&a1, &a2));
        const char *argv2_const[9] = {"prog", "-a",  "-l",  "-S", "-B",
                                      "299",  "./a", "./b", NULL};
        char *argv2[9];
        argv2[argc] = NULL;
        for (int i = 0; i < argc; ++i) {
            argv2[i] = new char[strlen(argv2_const[i]) + 1];
            strcpy(argv2[i], argv2_const[i]);
        }
        REQUIRE(simpledu_args_ctor(&a2, argc, argv2) == EXIT_SUCCESS);
        REQUIRE(!simpledu_args_equal(&a1, &a2));
        for (int i = 0; i < argc; ++i) delete argv[i];
        for (int i = 0; i < argc; ++i) delete argv2[i];
        REQUIRE(simpledu_args_dtor(&a1) == EXIT_SUCCESS);
        REQUIRE(simpledu_args_dtor(&a2) == EXIT_SUCCESS);
    }
}
