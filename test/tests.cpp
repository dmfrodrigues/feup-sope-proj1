#define CATCH_CONFIG_MAIN
#include "catch.hpp"

extern "C" {
    #include "simpledu_args.h"
}

TEST_CASE("simpledu_args_ctor", "[simpledu_args_ctor]") {
	{
        char *argv[] = {};
        simpledu_args_t args;
        REQUIRE(simpledu_args_ctor(&args, 0, argv) == EXIT_SUCCESS);
		REQUIRE(args.all           == false);
        REQUIRE(args.block_size    ==  1024);
        REQUIRE(args.count_links   == false);
        REQUIRE(args.dereference   == false);
        REQUIRE(args.separate_dirs == false);
        REQUIRE(args.max_depth     >= PATH_MAX/2);
    }
}
