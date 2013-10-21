#include "stdafx.h"
#include <tdk/event_loop/io/ip/tcp/pipeline/pipeline_builder.hpp>

namespace tdk {
namespace io {
namespace ip {
namespace tcp {

pipeline_builder::pipeline_builder( void ) {

}

pipeline_builder::~pipeline_builder( void ) {

}

std::error_code pipeline_builder::build( pipeline& p ) {
	return std::error_code();
}

const tdk::io::ip::tcp::config& pipeline_builder::config( void ) {
	return tdk::io::ip::tcp::config::default_config();
}

}}}}
