// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the TDKCORE_EXPORTS
// symbol defined on the command line. This symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// TDKCORE_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifndef __tdk_h__
#define __tdk_h__

#include <tdk/tdk_define.hpp>
#include <tdk/event_loop/event_loop.hpp>
#include <tdk/buffer/memory_block.hpp>
#include <tdk/io/ip/socket.hpp>
#include <tdk/io/ip/tcp/channel.hpp>
#include <tdk/io/ip/tcp/channel_connector.hpp>
#include <tdk/io/ip/tcp/channel_acceptor.hpp>
#include <tdk/io/ip/tcp/pipeline/filter.hpp>
#include <tdk/io/ip/tcp/pipeline/pipeline_builder.hpp>
#include <tdk/util/string.hpp>
#include <tdk/error/last_error.hpp>
#include <tdk/error/error_tdk.hpp>
#include <tdk/error/error_platform.hpp>
#include <tdk/log/logger.hpp>
#if defined (_WIN32)
#include <tdk/log/writer/console_writer.hpp>
#else
#include <tdk/log/writer/stderr_writer.hpp>
#endif

#include <tdk/threading/spin_lock.hpp>
#include <tdk/time/date_time.hpp>
#include <tdk/util/string.hpp>

#endif



