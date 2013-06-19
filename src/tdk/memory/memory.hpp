#ifndef __tdk_memory_h__
#define __tdk_memory_h__

namespace tdk { namespace memory {
namespace {
	static const uint32_t  ALLOC_SIZE_TABLE[] = { 4 , 8 , 16 , 32 , 64 , 128 , 256 , 512 , 1024 , 2048 , 4096 , 16384 , 65536 , 262144 };
	static const uint32_t  ALLOC_SIZE_TABLE_COUNT = _countof( ALLOC_SIZE_TABLE );
}

union block_info {
	block_info( bool variable , std::size_t sz );
	~block_info( void );
	 
	bool is_variable( void );
	bool is_valid( void );
	std::size_t size( void );

	bool is_free_marked( void );
	void free_mark( bool mark );
	
	void* align;
	std::size_t data;
};

}}

#endif