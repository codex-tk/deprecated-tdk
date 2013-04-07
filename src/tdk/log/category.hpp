#ifndef __tdk_log_category_h__
#define __tdk_log_category_h__

#include <string>

namespace tdk {
namespace log {

class category{
public:
	explicit category( const std::string& name );
	~category( void );

	const std::string& name( void ) const;

	const uint32_t hash( void ) const;
private:
	// for fast compare
	uint32_t _hash;
	std::string _name;
};

}}

#endif