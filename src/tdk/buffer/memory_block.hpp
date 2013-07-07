#ifndef __tdk_buffer_memory_block_h__
#define __tdk_buffer_memory_block_h__

#include <tdk/buffer/buffer_base.hpp>
#include <tdk/buffer/marker.hpp>

namespace tdk {
namespace buffer {

//! read / write 연산의 인터페이스 역활
/*!
	버퍼의 관리는 T_storage 에서 처리한다.
	0 <= _read_pos <= _write_pos <= size() 
*/
class memory_block {
public:
	//! 생성자 
	memory_block( void ) ;
	//! 버퍼를 할당하는 생성자
	explicit memory_block( const std::size_t size 
		, tdk::buffer::allocator* allocator = nullptr );
    memory_block( void* p 
		, std::size_t buf_size 
		, std::size_t data_size = 0 
		, tdk::buffer::allocator* allocator = nullptr );
	//! 복사 생성자
	memory_block( const memory_block& rhs ) ;
	//! 대입 연산자/
	memory_block& operator=( const memory_block& rhs );

	//! 복사 생성자
	memory_block( memory_block&& rhs ) ;
	//! 대입 연산자/
	memory_block& operator=( memory_block&& rhs );
	//! 소멸자 
	~memory_block( void );

	//! 버퍼의 크기
	std::size_t size( void ) const ;	
	//! 쓰기 가능한 크기
	std::size_t space( void ) const ;


	//! 쓰기 가능한 포인터
	uint8_t* wr_ptr( void ) ;
	//! 쓰기 가능한 포인터를 앞/뒤로 옮김
	int32_t wr_ptr( int32_t move );
	void wr_ptr( uint8_t* ptr );

	//! 읽기 가능한 사이즈
	std::size_t length( void ) const ;
	void length( std::size_t l );


	//! 읽기 가능한 포인터
	uint8_t* rd_ptr( void  ) const;
	//! 읽기 가능한 포인터를 앞/뒤로 옮김
	int32_t rd_ptr( int32_t move ) ;
	void rd_ptr( uint8_t* ptr ) ;

	//! 데이터의 시작점을 버퍼의 시작점에 맞추는 함수
	void align( void ) ;
	//! 버퍼를 늘리거나 독립버퍼로 만드는 함수
	void reserve( std::size_t sz ) ;
	//! 데이터 초기화 
	void clear( void ) ;
	void swap( memory_block& rhs ) ;
	int ref_count( void ) ;	
	marker mark( void ) ;	
	bool move( const marker& m ) ;
	std::size_t copy( void* p , std::size_t wr_size) ;
	//! 버퍼에 데이터 기록
	std::size_t write( void* buf , std::size_t write_size ) ;
	//! 데이터의 읽음 표시 없이 읽어오는 함수
	std::size_t peek( void* buf , std::size_t peek_size ) ;
	//! 데이터를 읽어오는 함수
	std::size_t read( void* buf , std::size_t read_size ) ;
	/*
	template < typename T >
	memory_block& operator<<( const T& v ) {
		write( static_cast< void* >(
					const_cast< T* >(&v))
				, sizeof( v ));
		return *this;
	}*/

	std::size_t write( const char* msg );
private:
	tdk::buffer::buffer_base _base;
	std::size_t _read_pos;
	std::size_t _write_pos;
};


template < typename T >
memory_block& operator << ( memory_block& buffer , const T& v ) {
	buffer.write( static_cast< void* >(
					const_cast< T* >(&v))
				, sizeof( v ));
	return buffer;
}

template < typename T  >
memory_block& operator >> ( memory_block& buffer , T& v ) {
	buffer.read( &v , sizeof( v ));
	return buffer;
}

}}

#endif
