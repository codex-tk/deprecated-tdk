#ifndef __tdk_io_ip_udp_pipeline_builder_h__
#define __tdk_io_ip_udp_pipeline_builder_h__

#include <system_error>

namespace tdk {
namespace io {
namespace ip {
namespace udp {

class pipeline;
class pipeline_builder {
public:
	pipeline_builder( void );
	virtual ~pipeline_builder( void );

	virtual std::error_code build( pipeline& p );
private:
};

}}}}

#endif
