#ifndef __tdk_log_category_h__
#define __tdk_log_category_h__

#include <string>
#include <tdk/tdk.hpp>

namespace tdk {
namespace log {

class category{
public:
	explicit category( const tdk::tstring& name );
	~category( void );

	const tdk::tstring& name( void ) const;

	const uint32_t hash( void ) const;
private:
	// for fast compare
	uint32_t _hash;
	tdk::tstring _name;
};

}}

#endif