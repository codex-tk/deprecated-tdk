#include "stdafx.h"
#include <tdk/buffer/memory_block.hpp>

TEST( buffer , capture ){

	tdk::buffer::memory_block mb(1024);

	ASSERT_EQ( mb.ref_count() , 1 );
	{
		auto capture = [mb](){
		};
		ASSERT_EQ( mb.ref_count() , 2 );
	}
	ASSERT_EQ( mb.ref_count() , 1 );
}