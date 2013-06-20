#ifndef __tdk_util_ini_section_h__
#define __tdk_util_ini_section_h__

#include <tdk/util/win32/ini_key.hpp>

namespace tdk {
namespace util {
class ini_file;
class ini_section{
public:    
    ini_section( const ini_file& file , const tdk::tstring& section );
    ~ini_section(void);

	ini_key operator[]( const tdk::tstring& key );

    const ini_file& file( void ) const;
    const tdk::tstring& name( void ) const;    	
private:
    const ini_file& _ini_file;
    tdk::tstring _section;
};


}}

#endif