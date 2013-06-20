#ifndef __tdk_config_IniKey_h__
#define __tdk_config_IniKey_h__

namespace tdk {
namespace util {

class ini_section;

class ini_key{
public:
    ini_key( const ini_section& section ,const tdk::tstring& key );
    ~ini_key(void);

	tdk::tstring to_string();
    int          to_int();

    void operator=( const tdk::tstring& value );
    void operator=( const int value );
private:
	const ini_section& _section;
    tdk::tstring _key;    
};

}}


#endif