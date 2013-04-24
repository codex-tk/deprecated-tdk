#include "Stdafx.h"
#include "logger.hpp"
#include <msclr\marshal_cppstd.h>
#include <tdk/log/writer/console_writer.hpp>
#include <tdk/util/string.hpp>

using namespace msclr::interop;

namespace tdk {
namespace clr {
namespace log {

logger::logger( System::String^ cate )
	: _impl( tdk::log::logger::impl::get_instance( marshal_as<std::wstring>(cate)))
{

}

logger::~logger( void ) {
	this->!logger();
}

logger::!logger( void ) {
	_impl = nullptr;
}

static void write_internal( tdk::log::logger::impl* impl , tdk::log::level lv , const std::wstring& msg ){
	tdk::log::logger log( impl );
	log.write( lv , L"" , 0 , L"" , msg.c_str());
}

void logger::debug( System::String^ msg , ... array<System::Object^>^ args ) {
	std::wstring mm = marshal_as<std::wstring>(System::String::Format( msg , args ));
	write_internal( _impl , tdk::log::level::log_debug , mm );
}

void logger::info( System::String^ msg , ... array<System::Object^>^ args ) {
	std::wstring mm = marshal_as<std::wstring>(System::String::Format( msg , args ));
	write_internal( _impl , tdk::log::level::log_info , mm );
}

void logger::warn( System::String^ msg , ... array<System::Object^>^ args ) {
	std::wstring mm = marshal_as<std::wstring>(System::String::Format( msg , args ));
	write_internal( _impl , tdk::log::level::log_warn , mm );
}

void logger::error( System::String^ msg , ... array<System::Object^>^ args ) {
	std::wstring mm = marshal_as<std::wstring>(System::String::Format( msg , args ));
	write_internal( _impl , tdk::log::level::log_error , mm );
}

void logger::fatal( System::String^ msg , ... array<System::Object^>^ args ) {
	std::wstring mm = marshal_as<std::wstring>(System::String::Format( msg , args ));
	write_internal( _impl , tdk::log::level::log_fatal , mm );
}

void logger::add_writer( tdk::clr::log::writer^ writer ) {
	tdk::log::writer_ptr ptr = *(writer->writer_ptr());
	tdk::log::logger log( _impl );
	log.add_writer( ptr );
}

void logger::level( enum level lv ) {
	_impl->level = (tdk::log::level)lv;
}
}}}
