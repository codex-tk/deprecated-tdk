#ifndef __tdk_fixed_size_allocator_h__
#define __tdk_fixed_size_allocator_h__

#include <tdk/threading/spin_lock.hpp>
#include <tdk/memory/memory.hpp>
#include <vector>

namespace tdk { namespace memory {

template < 
	typename T_lock ,
	typename T_base_allocator
>
class fixed_size_allocator {
public:
	fixed_size_allocator( void )
		: _alloc_size(0)
		, _contain_bytes(0){
	}
	
	~fixed_size_allocator( void ){
		tdk::threading::scoped_lock< T_lock > guard( _lock );
		while ( !_pool.empty() ){
			void* ptr = _pool.back();
			_pool.pop_back();
			T_base_allocator::free( ptr );
		}
	}

	void init( const std::size_t alloc_sz 
		, const std::size_t contain_bytes = 1024768 ) 
	{
		_alloc_size = alloc_sz;
		_contain_bytes = contain_bytes;
	}
	
	void* alloc( 
#if defined ( ENABLE_MEMORY_STATISTICS )
		tdk::memory::statistics& statistics
#else
		void 
#endif
	) {
#if defined ( ENABLE_MEMORY_STATISTICS )
		tdk::threading::atomic64::increment( &statistics.alloc_req_count );
#endif
		assert_msg( _alloc_size != 0 
			, "[fixed_size_allocator::alloc()] _alloc_size is 0" );
		void* ptr = nullptr;
		if ( !_pool.empty()) {
			tdk::threading::scoped_lock< T_lock > guard( _lock );
			if ( !_pool.empty() ) {
				ptr = _pool.back();
				_pool.pop_back();
				assert_msg( ptr != nullptr 
					, "[fixed_size_allocator::alloc()] pooled ptr is nullptr" );
			}
		}
		if ( ptr == nullptr ){
			ptr = T_base_allocator::alloc( _alloc_size );
			assert_msg( ptr != nullptr 
				, "[fixed_size_allocator::alloc()] alloc ptr is nullptr" );
#if defined ( ENABLE_MEMORY_STATISTICS )
			tdk::threading::atomic64::increment( &statistics.base_alloc_count );
#endif
		}
		return ptr;
	}
	
	void free( 
		void* p 
#if defined ( ENABLE_MEMORY_STATISTICS )
		, tdk::memory::statistics& statistics
#endif
	) {
		if ( p == nullptr )
			return;

#if defined ( ENABLE_MEMORY_STATISTICS )
		tdk::threading::atomic64::increment( &statistics.free_req_count );
#endif
		tdk::threading::scoped_lock< T_lock > guard( _lock );
		_pool.push_back( p );

#if defined ( ENABLE_MEMORY_STATISTICS )
		tdk::threading::atomic64::add( &statistics.pooling_bytes , _alloc_size );
#endif
		if ( _contain_bytes == 0 )
			return;

		if ( _alloc_size * _pool.size() > _contain_bytes ) {
#if defined ( ENABLE_MEMORY_STATISTICS )
			return_memory((int)(_contain_bytes * 0.8) , statistics );
			tdk::threading::atomic64::increment( &statistics.return_memory_call_count );
#else
			return_memory((int)(_contain_bytes * 0.8));
#endif
		}
	}

	void return_memory( 
		std::size_t remain_bytes 
#if defined ( ENABLE_MEMORY_STATISTICS )
		, tdk::memory::statistics& statistics
#endif
	) {
		while( _alloc_size * _pool.size() > remain_bytes ) {
			void* p = _pool.back();
			_pool.pop_back();
			T_base_allocator::free( p );
#if defined ( ENABLE_MEMORY_STATISTICS )
			int add = static_cast<int>(_alloc_size);
			tdk::threading::atomic64::add( &statistics.pooling_bytes , -add );
			tdk::threading::atomic64::increment( &statistics.base_free_count );
#endif
		}
	}
private:
	T_lock _lock;
	std::vector< void* > _pool;
	std::size_t _alloc_size;
	std::size_t _contain_bytes;
};


}}


#endif