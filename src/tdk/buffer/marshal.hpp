#ifndef __tdk_buffer_marshal_h__
#define __tdk_buffer_marshal_h__

#include <tdk/tdk.hpp>
#if defined( linux ) || defined( __linux__ )
#include <endian.h>
#endif

namespace tdk {
namespace buffer {

enum class endian {
		big
	,	little
};

endian host_endian( void );

namespace detail {

struct host_to_host {
	template < typename R , typename T >
	static R value( T t );

	template<uint64_t,uint64_t> static uint64_t value( uint64_t value ) {
		return value;
	}

	template<uint32_t,uint32_t> static uint32_t value( uint32_t value ) {
		return value;
	}

	template<uint16_t,uint16_t> static uint16_t value( uint16_t value ) {
		return value;
	}

	template<uint64_t> static uint64_t value( double value ) {
		uint64_t val;
		memcpy(&val,&value,sizeof(val));
		return val;
	}

	template<uint32_t,uint32_t> static uint32_t value( float value ) {
		uint32_t val;
		memcpy(&val,&value,sizeof(val));
		return val;
	}
	template <> static double value<double,uint64_t>( uint64_t value ) {
		double val;
		memcpy(&val,&value,sizeof(val));
		return val;
	}
	template <> static float value( uint32_t value ) {
		float val;
		memcpy(&val,&value,sizeof(val));
		return val;
	}
};


struct host_to_network {
	template < typename R , typename T >
	static R value( T t );

	template<> static uint64_t value( uint64_t value ) {
#if defined( _WIN32 )
		return htonll( value );
#else
		return htobe64( value );
#endif
	}

	template<> static uint32_t value( uint32_t value ) {
		return htonl( value );
	}

	template<> static uint16_t value( uint16_t value ) {
		return htons( value );
	}

	template<> static uint64_t value( double value ) {
#if defined( _WIN32 )
		return htond( value );
#else
		return htobe64( (uint64_t)value );
#endif
	}

	template<> static uint32_t value( float value ) {
#if defined( _WIN32 )
		return htonf( value );
#else
		return htonl( (uint32_t) value );
#endif
	}
};

struct network_to_host {
	template < typename R , typename T >
	static R value( T t );

	template <> static uint64_t value( uint64_t value ) {

#if defined( _WIN32 )
		return ntohll( value );
#else
		return be64toh( value );
#endif
	}

	template <> static uint32_t value( uint32_t value ) {
		return ntohl( value );
	}
	template <> static uint16_t value( uint16_t value ) {
		return ntohs( value );
	}
	template <> static double value( uint64_t value ) {
#if defined( _WIN32 )
		return ntohd( value );
#else
		return (double)be64toh( value );
#endif

	}
	template <> static float value( uint32_t value ) {
#if defined( _WIN32 )
		return ntohf( value );
#else
		return (float)ntohl(  value );
#endif
	}
};
}

template < typename T , typename C = tdk::buffer::detail::network_to_host >
class marshal_reader {
public:
	marshal_reader( T& t )
		: _buffer( t ) 
	{
	}

	~marshal_reader( void ) {
	}
	
	template < typename T >
	bool is_readable(void) {
		return _buffer.rd_size() >= sizeof( T );
	}

	T& buffer( void ) {
		return _buffer;
	}

	template < typename T >
	marshal_reader& operator>> ( T& t ) {
		_buffer >> t;
		t = C::value<T,T>(t);
		return *this;
	}

	template <>
	marshal_reader& operator>> ( double& t ) {
		uint64_t read;
		_buffer >> read;
		t = C::value<double,uint64_t>(read);
		return *this;
	}

	template <>
	marshal_reader& operator>> ( float& t ) {
		uint32_t read;
		_buffer >> read;
		t = C::value<float,uint32_t>(read);
		return *this;
	}

	template <>
	marshal_reader& operator>> ( uint8_t& t ) {
		_buffer >> t;
		return *this;
	}
private:
	T& _buffer;
};

template < typename T ,  typename C = tdk::buffer::detail::host_to_network>
class marshal_writer {
public:
	marshal_writer( T& t )
		: _buffer(t)
	{
	}

	~marshal_writer( void ) {
	}

	T& buffer( void ) {
		return _buffer;
	}
	
	template < typename T >
	marshal_writer& operator<< ( const T& t ) {
		_buffer << C::value<T,T>(t);
		return *this;
	}

	template <>
	marshal_writer& operator<< ( const double& t ) {
		_buffer << C::value<uint64_t,double>(t);
		return *this;
	}

	template <>
	marshal_writer& operator<< ( const float& t ) {
		_buffer << C::value<uint32_t,float>(t);
		return *this;
	}

	template <>
	marshal_writer& operator<< ( const uint8_t& t ) {
		_buffer << t;
		return *this;
	}
private:
	T& _buffer;
};

}}

#endif
