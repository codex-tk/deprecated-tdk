#ifndef __tdk_util_conf_h__
#define __tdk_util_conf_h__

namespace tdk { 
namespace util {

class conf{
public:
	conf( const tdk::tstring& file );
	~conf(void);

	bool load( void );

	tdk::tstring operator[]( const tdk::tstring& str );
private:
	tdk::tstring _filename;
	std::map< tdk::tstring , tdk::tstring > _value;
};

}}

#endif