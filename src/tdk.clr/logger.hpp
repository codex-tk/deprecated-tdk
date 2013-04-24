#pragma once

#include <tdk/log/logger.hpp>

#include "file_writer.hpp"

namespace tdk {
namespace clr {
namespace log {

public enum level {
	log_debug = 0,
	log_info  ,
	log_warn  ,
	log_error ,
	log_fatal ,
};

public ref class logger {
public:
	logger( System::String^ cate );
	~logger( void );
	!logger( void );

	void debug( System::String^ msg , ... array<System::Object^>^ args );
	void info( System::String^ msg , ... array<System::Object^>^ args );
	void warn( System::String^ msg , ... array<System::Object^>^ args );
	void error( System::String^ msg , ... array<System::Object^>^ args );
	void fatal( System::String^ msg , ... array<System::Object^>^ args );

	void add_writer( tdk::clr::log::writer^ writer );
	void level( enum level lv );
private:
	tdk::log::logger::impl* _impl;
};

}}}