#ifndef __tdk_buffer_memory_block_h__
#define __tdk_buffer_memory_block_h__

#include <tdk/buffer/buffer_base.hpp>
#include <tdk/buffer/marker.hpp>

namespace tdk {
namespace buffer {

//! read / write ������ �������̽� ��Ȱ
/*!
	������ ��� T_storage ���� ó���Ѵ�.
	0 <= _read_pos <= _write_pos <= size() 
*/
class memory_block {
public:
	//! ���� 
	memory_block( void ) ;
	//! ���۸� �Ҵ��ϴ� ����
	explicit memory_block( const std::size_t size 
		, tdk::buffer::allocator* allocator = nullptr );
    memory_block( void* p 
		, std::size_t buf_size 
		, std::size_t data_size = 0 
		, tdk::buffer::allocator* allocator = nullptr );
	//! ���� ����
	memory_block( const memory_block& rhs ) ;
	//! ���� ������/
	memory_block& operator=( const memory_block& rhs );

	//! ���� ����
	memory_block( memory_block&& rhs ) ;
	//! ���� ������/
	memory_block& operator=( memory_block&& rhs );
	//! �Ҹ��� 
	~memory_block( void );

	//! ������ ũ��
	std::size_t size( void ) const ;	
	//! ���� ������ ũ��
	std::size_t space( void ) const ;


	//! ���� ������ ������
	uint8_t* wr_ptr( void ) ;
	//! ���� ������ �����͸� ��/�ڷ� �ű�
	int32_t wr_ptr( int32_t move );
	void wr_ptr( uint8_t* ptr );

	//! �б� ������ ������
	std::size_t length( void ) const ;
	void length( std::size_t l );


	//! �б� ������ ������
	uint8_t* rd_ptr( void  ) const;
	//! �б� ������ �����͸� ��/�ڷ� �ű�
	int32_t rd_ptr( int32_t move ) ;
	void rd_ptr( uint8_t* ptr ) ;

	//! �������� �������� ������ �������� ���ߴ� �Լ�
	void align( void ) ;
	//! ���۸� �ø��ų� �������۷� ����� �Լ�
	void reserve( std::size_t sz ) ;
	//! ������ �ʱ�ȭ 
	void clear( void ) ;
	void swap( memory_block& rhs ) ;
	int ref_count( void ) ;	
	marker mark( void ) ;	
	bool move( const marker& m ) ;
	std::size_t copy( void* p , std::size_t wr_size) ;
	//! ���ۿ� ������ ���
	std::size_t write( void* buf , std::size_t write_size ) ;
	//! �������� ���� ǥ�� ���� �о���� �Լ�
	std::size_t peek( void* buf , std::size_t peek_size ) ;
	//! �����͸� �о���� �Լ�
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
	std::size_t write( const wchar_t* msg );
    int padding( int align , uint8_t val = 0 );
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

memory_block& operator << ( memory_block& buffer , const memory_block&  data ) ;

}}

#endif
