#include <tdk/event_loop/io/ip/tcp/pipeline/pipeline.hpp>

namespace tdk {
namespace io {
namespace ip {
namespace tcp {

pipeline::pipeline( tdk::event_loop& loop )
	: _loop(loop)
{

}

pipeline::~pipeline( void ) {

}

tdk::event_loop& pipeline::loop( void ) {
	return _loop;
}

}}}}
