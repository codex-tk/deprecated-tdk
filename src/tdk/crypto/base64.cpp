#include "StdAfx.h"
#include "base64.hpp"
#include <limits>
namespace tdk {
namespace crypto {
	
namespace {
static const char b64_table[65] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

static const char reverse_table[128] = {
   64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
   64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
   64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 62, 64, 64, 64, 63,
   52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 64, 64, 64, 64, 64, 64,
   64,  0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14,
   15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 64, 64, 64, 64, 64,
   64, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40,
   41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 64, 64, 64, 64, 64
};

}

base64::base64(void){}
base64::~base64(void){}

std::string base64::encode( const std::string& msg ) {
	return encode( (void*)msg.c_str() , msg.length() );
}

std::string base64::encode( void* ptr , uint32_t size) {
	using ::std::numeric_limits;

	if ( size  > 1024968 ){
	//if ( size > (std::numeric_limits<std::string::size_type>::max() / 4u) * 3u) {
		return "";
	}

	std::string retval((((size + 2) / 3) * 4), '=');
	std::size_t outpos = 0;
	int bits_collected = 0;
	unsigned int accumulator = 0;
	unsigned char* buffer = static_cast< unsigned char* >( ptr );
	for ( uint32_t i = 0; i < size ; ++i  ) {
		accumulator = (accumulator << 8) | (buffer[i] & 0xff);
		bits_collected += 8;
		while (bits_collected >= 6) {
			bits_collected -= 6;
			retval[outpos++] = b64_table[(accumulator >> bits_collected) & 0x3fu];
		}
	}
	if (bits_collected > 0) { 
		accumulator <<= 6 - bits_collected;
		retval[outpos++] = b64_table[accumulator & 0x3fu];
	}
	return retval;
}

int base64::decode( const std::string& src , void* ptr , uint32_t size ) {
	int index = 0 ;
	unsigned char* buffer = static_cast< unsigned char* >(ptr);
	const std::string::const_iterator last = src.end();
	int bits_collected = 0;
	unsigned int accumulator = 0;

	for (std::string::const_iterator i = src.begin(); i != last; ++i) {
		const int c = *i;
		if ( isspace(c) || c == '=') {
			continue;
		}
		if ((c > 127) || (c < 0) || (reverse_table[c] > 63)) {
			//
		}
		accumulator = (accumulator << 6) | reverse_table[c];
		bits_collected += 6;
		if (bits_collected >= 8) {
			bits_collected -= 8;
			buffer[index++] = (char)((accumulator >> bits_collected) & 0xff);
		}
	}
	return index;
}

}}