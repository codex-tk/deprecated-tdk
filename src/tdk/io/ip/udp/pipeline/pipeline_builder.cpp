#include "stdafx.h"
#include <tdk/io/ip/udp/pipeline/pipeline_builder.hpp>

namespace tdk {
namespace io {
namespace ip {
namespace udp {

pipeline_builder::pipeline_builder( void ) {

}

pipeline_builder::~pipeline_builder( void ) {

}

std::error_code pipeline_builder::build( pipeline& p ) {
	return std::error_code();
}

}}}}
