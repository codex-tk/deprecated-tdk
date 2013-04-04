#include "stdafx.h"
#include <tdk/threading/atomic/memory_order.hpp>

namespace tdk {
namespace threading {


void acquire::acquire_barrier( void ) {
}


void no_acquire::acquire_barrier( void ) {

}


void release::release_barrier( void ) {

}

void no_release::release_barrier( void ) {

}

}}