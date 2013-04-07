#include "stdafx.h"
#include <tdk/log/writer/writer.hpp>

namespace tdk{
namespace log{

writer::writer( const formatter_ptr& fmt ) 
	: _formatter( fmt ){
}

writer::~writer( void ) {

}

}}
