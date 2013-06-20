#ifndef __tdk_util_ini_file_h__
#define __tdk_util_ini_file_h__

#include <tdk/util/win32/ini_section.hpp>
namespace tdk {
namespace util {

class ini_file{
public:
    ini_file( const tdk::tstring& filename , const tdk::tstring& defstring = _T("")  , const int defint = -1);
    ~ini_file(void);

    ini_section operator[]( const tdk::tstring& section );

    const tdk::tstring& name( void ) const;

    bool is_exist( void ) const;

	template < typename T >
	T default(void) const ;

	template <>
	int default<int>( void ) const {
		return _def_int;
	}

	template <>
	tdk::tstring default<tdk::tstring>() const {
		return _def_string;
	}

private:
	tdk::tstring _file_name;
    tdk::tstring _def_string;
    int          _def_int;
};

}}

#endif
