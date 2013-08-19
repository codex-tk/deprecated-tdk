#ifndef __tdk_log_file_writer_h__
#define __tdk_log_file_writer_h__

#include <tdk/log/writer/writer.hpp>
#include <tdk/log/record.hpp>
#include <tdk/threading/spin_lock.hpp>

namespace tdk {
namespace log {
#if defined (_WIN32)
class file_writer : public writer {
public:
	file_writer( const std::wstring name 
		, const tdk::log::formatter_ptr& fmt );
	virtual ~file_writer( void );
	virtual void write( const record& r );
private:
	bool _create_log_file();
	void _delete_old_logs();
private:
	tdk::buffer::memory_block _buffer;	
	tdk::threading::spin_lock _lock;
	HANDLE			_file;
    std::wstring	_poxt_fix;
	uint32_t		_current_day;
	uint32_t		_line_count;
	uint32_t		_day_file_count;
};
#else
class file_writer : public writer {
public:
    file_writer( void );
    file_writer( const std::string name );
	file_writer( const std::string name 
		, const tdk::log::formatter_ptr& fmt );
	virtual ~file_writer( void );
	virtual void write( const record& r );
private:
	bool _create_log_file();
	void _delete_old_logs();
private:
	tdk::buffer::memory_block _buffer;	
	tdk::threading::spin_lock _lock;
	FILE*           _file;
    std::string	    _poxt_fix;
	uint32_t		_current_day;
	uint32_t		_line_count;
	uint32_t		_day_file_count;
};
#endif
}}

#endif
