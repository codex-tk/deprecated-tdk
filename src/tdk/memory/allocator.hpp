#ifndef __tdk_allocator_h__
#define __tdk_allocator_h__

#include <tdk/memory/memory.hpp>
#include <tdk/memory/fixed_size_allocator.hpp>

namespace tdk { namespace memory {
	
template < 
	typename T_lock ,
	typename T_base_allocator ,
	template < typename , typename > class T_fixed_size_allocator
> 
class allocator {
public:
	allocator( void ) {
		for ( int i = 0; i < ALLOC_SIZE_TABLE_COUNT ; ++i ) {
			_allocator[i].init(
				ALLOC_SIZE_TABLE[i] + sizeof( block_info )
				, 1024 * 1024 * 5 );
		}
	}

	~allocator( void ) {
	
	}

	void* alloc( std::size_t sz ) {
		for ( int i = 0; i < ALLOC_SIZE_TABLE_COUNT ; ++i ) {
			if ( sz <= ALLOC_SIZE_TABLE[i] )
				return _alloc_fixed_block(i);
		}
		return _alloc_variable_block( sz );
	}

	void free( void* p ) {
		if ( p == nullptr ) {
			TDK_ASSERT( !"[allocator::free()] param p is nullptr" );
			return;
		}
		block_info* bi = static_cast<block_info*>(p) - 1;
		if ( !bi->is_valid()) {
			TDK_ASSERT( !"[allocator::free()] block_info not valid");
			return;
		}
		if ( bi->is_variable() ) {
			T_base_allocator::free( bi );
			return;
		}
		std::size_t idx = bi->size();
		if ( idx < 0 || idx >= ALLOC_SIZE_TABLE_COUNT ) {
			TDK_ASSERT( !"[allocator::free()] block_info index error" );
			return;
		}
#if defined ( ENABLE_MEMORY_STATISTICS )
		_allocator[idx].free( bi , statistics::base_statistics[idx] );
#else 
		_allocator[idx].free( bi );
#endif
	}
	
	void* realloc( void* p , std::size_t sz ) {
		if ( p == nullptr )
            return this->alloc( sz );

        std::size_t buf_size = this->size( p );
		
        if ( sz <= buf_size )
            return p;
		
        void* ptr = this->alloc( sz );		

		if ( ptr != nullptr )
			memcpy( ptr , p , buf_size );

        this->free( p );
		return ptr;	
	}

	std::size_t size( void* p ) {
		if ( p == nullptr ) {
			TDK_ASSERT( !"[allocator::size()] param p is nullptr" );
			return 0;
		}
		block_info* bi = static_cast<block_info*>(p) - 1;
		if ( !bi->is_valid()) {
			TDK_ASSERT( !"[allocator::size()] block_info not valid");
			return 0;
		}
		if ( bi->is_variable() ) {
			return bi->size();;
		}
		std::size_t idx = bi->size();
		if ( idx < 0 || idx >= ALLOC_SIZE_TABLE_COUNT ) {
			TDK_ASSERT( !"[allocator::size()] block_info index error" );
			return 0;
		}
		return ALLOC_SIZE_TABLE[idx];
	}
private:
	void* _alloc_fixed_block( int idx ) {
#if defined ( ENABLE_MEMORY_STATISTICS )
		block_info* bi = static_cast< block_info* >(
			_allocator[idx].alloc( statistics::base_statistics[idx] ));
#else
		block_info* bi = static_cast< block_info* >(
			_allocator[idx].alloc());
#endif		
		new (bi) block_info( false , idx );
		return bi + 1;
	}

	void* _alloc_variable_block( std::size_t sz ) {
		block_info* bi = static_cast< block_info* >(T_base_allocator::alloc( sz ));
		new (bi) block_info( true , sz );
		return bi + 1;
	}
private:
	T_fixed_size_allocator< T_lock , T_base_allocator > _allocator[ALLOC_SIZE_TABLE_COUNT];
};


}}


#endif