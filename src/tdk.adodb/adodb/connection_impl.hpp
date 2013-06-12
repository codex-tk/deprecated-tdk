#ifndef __tdk_adodb_connection_impl_h__
#define __tdk_adodb_connection_impl_h__

#import "c:/Program Files/Common Files/System/ado/msado15.dll" no_namespace rename("EOF", "adoEOF")

namespace tdk {
namespace adodb {

struct connection_impl {
	_ConnectionPtr connection_ptr;
};

struct record_set_impl {
	_ConnectionPtr connection_ptr;
	_RecordsetPtr record_set_ptr;
};

}}

#endif