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

//! ���� ����
memory_block::memory_block( memory_block&& rhs ) 
	: _base( std::move( rhs._base ))
	, _read_pos( rhs._read_pos )
	, _write_pos( rhs._write_pos ){
	rhs._read_pos = 0;
	rhs._write_pos = 0;
}

//! ���� ������/
memory_block& memory_block::operator=( memory_block&& rhs ) {
	_base = std::move( rhs._base );
	_read_pos = rhs._read_pos;
	_write_pos = rhs._write_pos;
	return *this;
}

memory_block::~memory_block( void ) {

}
std::size_t memory_block::size( void ) const {
	return _base.size();
}
	
//! ���� ������ ũ��
std::size_t memory_block::space( void ) const {
	return size() - _write_pos;
}

//! ���� ������ ������
uint8_t* memory_block::wr_ptr( void ) {
	return _base.data_ptr() + _write_pos;
}

//! ���� ������ �����͸� ��/�ڷ� �ű�
int32_t memory_block::wr_ptr( int32_t move ) {
	if ( move >= 0 ) {
		move = std::min( move , static_cast<int32_t>(space()));
	} else {
		move = std::min( abs( move ) , static_cast<int32_t>(length()));
		move = -move;
	}
	_write_pos += move;
	return move;
}

void memory_block::wr_ptr( uint8_t* ptr ) {
	_write_pos = ptr - _base.data_ptr();
}

//! �б� ������ ������
std::size_t memory_block::length( void ) const {
	return _write_pos - _read_pos;
}
	
void memory_block::length( std::size_t l ){
	_write_pos = _read_pos + l;
}
//! �б� ������ ������
uint8_t* memory_block::rd_ptr( void  ) const {
	return _base.data_ptr() + _read_pos;
}

//! �б� ������ �����͸� ��/�ڷ� �ű�
int32_t memory_block::rd_ptr( int32_t move ) {
	if ( move >= 0 ) {
		move = std::min( move , static_cast<int32_t>(length()));
	} else {
		move = std::min( abs( move ) , static_cast< int32_t >(_read_pos));
		move = -move;
	}
	_read_pos += move;
	return move;
}

void memory_block::rd_ptr( uint8_t* ptr ) {
	_read_pos = ptr - _base.data_ptr();
}

//! �������� �������� ������ �������� ���ߴ� �Լ�
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

//! ���۸� �ø��ų� �������۷� ����� �Լ�
void memory_block::reserve( std::size_t sz ) {
	_base.reserve( sz );
	_read_pos = std::min( size() , _read_pos );
	_write_pos = std::min( size() , _write_pos );
}

//! ������ �ʱ�ȭ 
void memory_block::clear( void ) {
	_base.reserve( _base.size() );
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

//! ���ۿ� ������ ���
std::size_t memory_block::write( void* buf , std::size_t write_size ) {
	reserve( _write_pos + write_size );
	write_size = std::min( space() , write_size );
	memcpy( wr_ptr() , buf , write_size );
	return wr_ptr( static_cast<int>(write_size));
}

//! �������� ���� ǥ�� ���� �о���� �Լ�
std::size_t memory_block::peek( void* buf , std::size_t peek_size ) {
	peek_size = std::min( length() , peek_size );
	memcpy( buf , rd_ptr() , peek_size );
	return peek_size;
}

//! �����͸� �о���� �Լ�
std::size_t memory_block::read( void* buf , std::size_t read_size ) {
	read_size = peek( buf , read_size );
	return rd_ptr( static_cast<int>(read_size));
}

std::size_t memory_block::write_fmt( const char* msg , ... ) {
	char fmt_buffer[2048] = { 0 , };
	va_list args;
	va_start( args , msg );
#if defined(_WIN32) || defined(_WIN64)
	int len = _vsnprintf_s( fmt_buffer , 2048 , msg , args );
#else
	int len = vsnprintf( fmt_buffer , 2048 , msg , args );
#endif
	va_end( args );
	return write( fmt_buffer );
}

std::size_t memory_block::write( const char* msg ) {
	return write( const_cast<char*>(msg) , strlen(msg));
}

std::size_t memory_block::write( const wchar_t* msg ) {
	return write( const_cast<wchar_t*>( msg ) , wcslen( msg ) * sizeof( wchar_t ));
}

int memory_block::padding( int align , uint8_t val ) {
    int mod = length() %  align;
    if ( mod == 0 )
        return 0;

    int append = align - mod;
    while ( append > 0 ) {
        *this << val;    
        --append;
    }
    return align-mod;
}

memory_block& operator << ( memory_block& buffer , const memory_block&  data ) {
	buffer.write( data.rd_ptr() , data.length());
	return buffer;
}

}}

