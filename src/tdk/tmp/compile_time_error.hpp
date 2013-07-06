#ifndef __tdk_tmp_compile_time_error_h__
#define __tdk_tmp_compile_time_error_h__

namespace tdk {
namespace tmp {

template < bool > class compile_time_error;
template <> class compile_time_error< true >{};

}}


#endif