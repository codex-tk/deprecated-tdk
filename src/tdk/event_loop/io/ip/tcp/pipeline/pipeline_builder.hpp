#ifndef __tdk_io_ip_tcp_pipeline_builder_h__
#define __tdk_io_ip_tcp_pipeline_builder_h__


namespace tdk {
namespace io {
namespace ip {
namespace tcp {

class pipeline;
class pipeline_builder {
public:
	pipeline_builder( void );
	virtual ~pipeline_builder( void );

	virtual void build( );
private:
};

}}}}

#endif