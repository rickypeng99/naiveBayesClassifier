//
// Created by Ricky Peng on 3/12/18.
//


#define CATCH_CONFIG_MAIN
#include "../catch.hpp"
#include "../src/ImageData.h"
#include <iostream>



TEST_CASE( "Testing boolean transfer", "[testTransBool]" ) {
    REQUIRE(transferBool('#'));
    REQUIRE(transferBool('+'));
    REQUIRE(!transferBool(' '));


}



