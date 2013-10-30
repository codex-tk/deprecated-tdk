#include "stdafx.h"
#include <tdk/task/queue.hpp>

namespace tdk {
namespace task {

queue::queue( void ) 
	: _running( nullptr )
{
}

queue::~queue( void ) {

}

void queue::execute( queue::operation* op ) {
	do {
		tdk::threading::scoped_lock<> guard( _lock );
		if ( _running ){
			_op_queue.add_tail( op );
			return;
		} else {
			_running = op;
		}
	} while(0);
	do {
		(*_running)();
	} while ( fetch());
}

bool queue::fetch( void ) {
	tdk::threading::scoped_lock<> guard( _lock );
	if ( !_op_queue.is_empty() ) {
		_running = _op_queue.front();
		_op_queue.pop_front();
		return true;
	}
	_running = nullptr;
	return false;
}

queue::operation::operation( queue* q )
	: _queue(q)
	, _handler( nullptr )
	, _context( nullptr )
{
}

queue::operation::~operation( void ){
}

void queue::operation::set_handler( 
	queue::operation::handler h ) 
{
	_handler = h;
}

void  queue::operation::context( void* ctx ) {
	_context = ctx;
}

void* queue::operation::context( void ) {
	return _context;
}

void queue::operation::dispatch( void ) {
	if (_queue){
		_queue->execute( this );
	}else{
		if ( _handler )
			(*_handler)( this );
	}
}

void queue::operation::operator()( void ) {
	if ( _handler )
		(*_handler)( this );
}

}}
