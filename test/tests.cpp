#define CATCH_CONFIG_MAIN
#include "catch.hpp"

extern "C" {
    #include "simpledu_args.h"
}

TEST_CASE("simpledu_args_ctor", "[simpledu_args_ctor]") {
	{
        const char *argv[] = {};
        simpledu_args_t args;
        REQUIRE(simpledu_args_ctor(&args, 0, argv) == EXIT_SUCCESS);
		REQUIRE(args.all           == false);
        REQUIRE(args.block_size    ==  1024);
        REQUIRE(args.count_links   == false);
        REQUIRE(args.dereference   == false);
        REQUIRE(args.separate_dirs == false);
        REQUIRE(args.max_depth     >= PATH_MAX/2);
        REQUIRE(args.filesc        == 1);
        REQUIRE(strcmp(args.files[0], ".") == 0);
    }
    {
        int argc = 7;
        const char *argv[7] = {"-a", "-l", "-S", "-B", "300", "./a", "./b"};
        simpledu_args_t args;
        REQUIRE(simpledu_args_ctor(&args, argc, argv) == EXIT_SUCCESS);
		REQUIRE(args.all           == true);
        REQUIRE(args.block_size    ==  300);
        REQUIRE(args.count_links   == true);
        REQUIRE(args.dereference   == false);
        REQUIRE(args.separate_dirs == true);
        REQUIRE(args.max_depth     >= PATH_MAX/2);
        REQUIRE(args.filesc        == 2);
        REQUIRE(strcmp(args.files[0], "./a") == 0);
        REQUIRE(strcmp(args.files[1], "./b") == 0);
    }
}

TEST_CASE("simpledu_args_equal", "[simpledu_args_equal]") {
    {
        int argc = 7;
        const char *argv[7] = {"-a", "-l", "-S", "-B", "300", "./a", "./b"};
        simpledu_args_t a1;
        REQUIRE(simpledu_args_ctor(&a1, argc, argv) == EXIT_SUCCESS);
        simpledu_args_t a2;
        REQUIRE(simpledu_args_ctor(&a2, argc, argv) == EXIT_SUCCESS);
		REQUIRE(simpledu_args_equal(&a1, &a2));
        argv[4] = "299";
        REQUIRE(simpledu_args_ctor(&a2, argc, argv) == EXIT_SUCCESS);
		REQUIRE(!simpledu_args_equal(&a1, &a2));
    }
}
