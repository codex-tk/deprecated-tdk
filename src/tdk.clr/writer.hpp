#pragma once

#include <tdk/log/writer/writer.hpp>

namespace tdk {
namespace clr {
namespace log {

public interface class writer {
	std::tr1::shared_ptr<tdk::log::writer>* writer_ptr( void );
};

}}}