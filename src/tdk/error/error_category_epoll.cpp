#include <tdk/error/error_category_epoll.hpp>
#include <tdk/util/string.hpp>

#include <sstream>
#include <tdk/error/error_platform.hpp>

namespace tdk {

error_category_epoll::error_category_epoll(void) {

}

error_category_epoll::~error_category_epoll(void) _NOEXCEPT {

}

const char *error_category_epoll::name() const _NOEXCEPT {
	return "epoll_category";
}

std::string error_category_epoll::message(int _Errval) const  {
    std::string err_msg = "[epoll error] " 
        + tdk::platform::category().message( _Errval );
    return err_msg;
}


}
