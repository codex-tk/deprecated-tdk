#include "Stdafx.h"
#include "file_writer.hpp"
#include <msclr\marshal_cppstd.h>
#include <tdk/log/writer/console_writer.hpp>
#include <tdk/util/string.hpp>
#include <tdk/log/formatter/string_formatter.hpp>
using namespace msclr::interop;

namespace tdk {
namespace clr {
namespace log {

file_writer::file_writer( System::String^ name )
	: _writer(  new std::tr1::shared_ptr<tdk::log::file_writer>(
					new tdk::log::file_writer(marshal_as<std::wstring>(name)
					, tdk::log::string_formatter::instance())))
{
	
}

file_writer::~file_writer( void ) {
	this->!file_writer();
}

file_writer::!file_writer( void ) {
	if ( _writer ) {
		delete _writer;
	}
	_writer = nullptr;
}
std::tr1::shared_ptr<tdk::log::file_writer>* file_writer::writer_ptr( void ){
	return _writer;
}

}}}
