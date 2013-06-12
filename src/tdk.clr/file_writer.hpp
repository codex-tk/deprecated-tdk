#pragma once

#include <tdk/log/writer/file_writer.hpp>
#include "writer.hpp"

namespace tdk {
namespace clr {
namespace log {

public ref class file_writer : public writer{
public:
	file_writer( System::String^ name );
	~file_writer( void );
	!file_writer( void );

	virtual std::tr1::shared_ptr<tdk::log::writer>* writer_ptr( void );
private:
	std::tr1::shared_ptr<tdk::log::writer>* _writer;
};

}}}