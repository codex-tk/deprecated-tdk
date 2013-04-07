#include "stdafx.h"
#include <tdk/buffer/memory_block.hpp>

namespace tdk {
namespace buffer {

memory_block::memory_block( void ) 
	: _base(0) 
	, _read_pos(0) 
	, _write_pos(0)
{
}
memory_block::memory_block( const std::size_t size 
	, tdk::buffer::allocator* allocator )
	: _base(size , allocator)
	, _read_pos(0) 
	, _write_pos(0)
{
}

memory_block::memory_block( void* p 
	, std::size_t buf_size 
	, std::size_t data_size 
	, tdk::buffer::allocator* allocator )
	: _base( p , buf_size , allocator )
	, _read_pos(0) 
	, _write_pos(data_size)
{
}

memory_block::memory_block( const memory_block& rhs )
	: _base( rhs._base )
	, _read_pos( rhs._read_pos )
	, _write_pos( rhs._write_pos ){
}

memory_block& memory_block::operator=( const memory_block& rhs ) {
	memory_block temp( rhs );
	swap( temp );
	return *this;
}

memory_block::~memory_block( void ) {

}
std::size_t memory_block::size( void ) const {
	return _base.size();
}
	
//! 쓰기 가능한 크기
std::size_t memory_block::space( void ) const {
	return size() - _write_pos;
}

//! 쓰기 가능한 포인터
uint8_t* memory_block::wr_ptr( void ) {
	return _base.data_ptr() + _write_pos;
}

//! 쓰기 가능한 포인터를 앞/뒤로 옮김
int32_t memory_block::wr_ptr( int32_t move ) {
	if ( move >= 0 ) {
		move = min( move , static_cast<int32_t>(space()));
	} else {
		move = min( abs( move ) , static_cast<int32_t>(length()));
		move = -move;
	}
	_write_pos += move;
	return move;
}

void memory_block::wr_ptr( uint8_t* ptr ) {
	_write_pos = ptr - _base.data_ptr();
}

//! 읽기 가능한 사이즈
std::size_t memory_block::length( void ) const {
	return _write_pos - _read_pos;
}
	
void memory_block::length( std::size_t l ){
	_write_pos = _read_pos + l;
}
//! 읽기 가능한 포인터
uint8_t* memory_block::rd_ptr( void  ) const {
	return _base.data_ptr() + _read_pos;
}

//! 읽기 가능한 포인터를 앞/뒤로 옮김
int32_t memory_block::rd_ptr( int32_t move ) {
	if ( move >= 0 ) {
		move = min( move , static_cast<int32_t>(length()));
	} else {
		move = min( abs( move ) , static_cast< int32_t >(_read_pos));
		move = -move;
	}
	_read_pos += move;
	return move;
}

void memory_block::rd_ptr( uint8_t* ptr ) {
	_read_pos = ptr - _base.data_ptr();
}

//! 데이터의 시작점을 버퍼의 시작점에 맞추는 함수
void memory_block::align( void ) {
	reserve( size() );
	if ( _write_pos != _read_pos ) {
		memmove( _base.data_ptr() , rd_ptr() , length() );
		_write_pos = length();
		_read_pos  = 0;
	} else {
		clear();
	}
}

//! 버퍼를 늘리거나 독립버퍼로 만드는 함수
void memory_block::reserve( std::size_t sz ) {
	_base.reserve( sz );
	_read_pos = min( size() , _read_pos );
	_write_pos = min( size() , _write_pos );
}

//! 데이터 초기화 
void memory_block::clear( void ) {
	_write_pos = 0;
	_read_pos  = 0;
}

void memory_block::swap( memory_block& rhs ) {
	_base.swap( rhs._base );
	std::swap( _read_pos , rhs._read_pos );
	std::swap( _write_pos , rhs._write_pos );
}

int memory_block::ref_count( void ) {
	return _base.ref_count();
}
	
marker memory_block::mark( void ) {
	return marker( _read_pos , _write_pos );
}
	
bool memory_block::move( const marker& m ) {
	if ( m.write_mark() < size() ) {
		_read_pos  = m.read_mark();
		_write_pos = m.write_mark();
		return true;
	}
	return false;
}

std::size_t memory_block::copy( void* p , std::size_t wr_size) {
	return write( p , wr_size );
}

//! 버퍼에 데이터 기록
std::size_t memory_block::write( void* buf , std::size_t write_size ) {
	reserve( _write_pos + write_size );
	write_size = min( space() , write_size );
	memcpy( wr_ptr() , buf , write_size );
	return wr_ptr( write_size );
}

//! 데이터의 읽음 표시 없이 읽어오는 함수
std::size_t memory_block::peek( void* buf , std::size_t peek_size ) {
	peek_size = min( length() , peek_size );
	memcpy( buf , rd_ptr() , peek_size );
	return peek_size;
}

//! 데이터를 읽어오는 함수
std::size_t memory_block::read( void* buf , std::size_t read_size ) {
	read_size = peek( buf , read_size );
	return rd_ptr( read_size );
}

}}

