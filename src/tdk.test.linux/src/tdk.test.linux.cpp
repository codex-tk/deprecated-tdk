//============================================================================
// Name        : linux.cpp
// Author      : tk
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
using namespace std;
#include <tdk/tdk.hpp>
#include <gtest/gtest.h>

int main( int argc , char* argv[] ) {
	tdk::init();
	testing::InitGoogleTest( &argc , argv );
	return RUN_ALL_TESTS();
}
