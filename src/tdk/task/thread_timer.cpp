#include <tdk/task/thread_timer.hpp>
namespace tdk {
namespace task {

thread_timer::thread_timer( void ){
    _thread = std::thread( [this]{
                on_timer();
            });
}

thread_timer::~thread_timer( void ){

}

void thread_timer::on_timer( void ) {
    while( true ) {

    }
}

}}

