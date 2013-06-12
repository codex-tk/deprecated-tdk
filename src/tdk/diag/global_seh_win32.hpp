#ifndef __tdk_diag_global_seh_h__
#define __tdk_diag_global_seh_h__

namespace tdk {
namespace diag {

class seh_handler{
public:
	seh_handler(void){}
	virtual ~seh_handler(void){}
	virtual void handle_exception( EXCEPTION_POINTERS *pExp ) = 0;
};

class global_seh {
public:
	global_seh( void );
	~global_seh( void );

	bool install( void );
	void register_handler( seh_handler* handler );
	void handle_exception( EXCEPTION_POINTERS *pExp );
private:
	std::vector< seh_handler* > _handlers;

};

}}


#endif