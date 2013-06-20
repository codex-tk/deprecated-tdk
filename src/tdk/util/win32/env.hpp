#ifndef __tdk_util_env_h__
#define __tdk_util_env_h__

namespace tdk {
namespace util {

class env {
public:
    enum class version {
		unknown                             = 0x00000000 , 
        windows_31                          = 0x00000001 , 
        windows_9x                          = 0x00000002 , 
        windows_95                          = 0x00000003 , 
        windows_95osr2                      = 0x00000004 , 
        windows_98                          = 0x00000005 , 
        windows_98se                        = 0x00000006 , 
        windows_me                          = 0x00000007 , 
        windows_nt                          = 0x00000009 , 
        windows_nt3                         = 0x0000000a , 
        windows_nt4                         = 0x0000000b , 
		windows_2000                        = 0x0000000c , 
        windows_xphome                      = 0x0000000d , 
		windows_xp                          = 0x0000000e , 
		windows_xpprofessionalx64edition    = 0x0000000f , 
		windows_server2003                  = 0x00000010 ,  
		windows_homeserver                  = 0x00000011 , 
		windows_server2003r2                = 0x00000012 , 
		windows_vista                       = 0x00000013 , 
		windows_server2008                  = 0x00000014 , 
		windows_server2008r2                = 0x00000015 , 
		windows_7                           = 0x00000016 , 
    };
public:
	static int				__stdcall number_of_process( void );
	static version			__stdcall os_version( void );
	static tdk::tstring		__stdcall process_name( void );
	static tdk::tstring		__stdcall process_directory( void );
	static tdk::tstring		__stdcall current_directory( void );
	static tdk::tstring		__stdcall process_path( void );
	static int				__stdcall page_size( void );

	static tdk::tstring		__stdcall os_version_string( void );
};

}}
#endif
