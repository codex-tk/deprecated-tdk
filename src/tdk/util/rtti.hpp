#ifndef __tdk_util_rtti_h__
#define __tdk_util_rtti_h__

namespace tdk {
namespace util {

class rtti {
public:
	template < typename t >
	static const rtti* get( void ) { return &t::s_rtti; }
	template < typename t >
	static const rtti* get( const t* obj ) { return obj != nullptr ?  obj->rtti() : nullptr ; }

	class get_handler {
	public:
		typedef const tdk::util::rtti* (*handler_function)();

		get_handler( void );
		~get_handler( void );

		const tdk::util::rtti* operator()( void ) const ;
		void set_handler( handler_function h ) ;
	private:
		handler_function _handler;
	};
public:
	rtti( const std::string& name , const rtti* base = nullptr );
	~rtti( void );

	inline const std::string& name( void ) const {
		return _name;
	}
	inline const rtti* base( void ) const {
		return _base;
	}
private:
	const std::string _name;
	const rtti* _base;
};
																							
#define DECL_ROOT_RTTI_WITH_OUT_VFTABLE( class_name )				\
public: 															\
	tdk::util::rtti::get_handler	 rtti;							\
	static const tdk::util::rtti	 s_rtti;						\
	static const tdk::util::rtti* _rtti_impl( void) {				\
		return &class_name::s_rtti;									\
	}																\
	template < typename type >										\
	bool is_kind_of() {												\
		const tdk::util::rtti* r = rtti();							\
		while ( r != nullptr ){										\
			if ( r == &type::s_rtti ) {								\
				return true;										\
			}														\
			r = r->base();											\
		}															\
		return false;												\
	}																\

#define IMPL_RTTI_CTOR_WITH_OUT_VFTABLE( class_name )				\
	rtti.set_handler( &class_name::_rtti_impl);						\
																	
#define DECL_CHILD_RTTI_WITH_OUT_VFTABLE( class_name )				\
	static const tdk::util::rtti	 s_rtti;						\
	static const tdk::util::rtti* _rtti_impl( void) {				\
		return &class_name::s_rtti;									\
	}																\
																
#define DECL_ROOT_RTTI( class_name )								\
public:																\
	static  const tdk::util::rtti  s_rtti;							\
	virtual const tdk::util::rtti* rtti( void ){					\
		return &class_name::s_rtti;									\
	}																\
	template < typename type >										\
	bool is_kind_of() {												\
		const tdk::util::rtti* r = rtti();							\
		while ( r != nullptr ){										\
			if ( r == &type::s_rtti ) {								\
				return true;										\
			}														\
			r = r->base();											\
		}															\
		return false;												\
	}																\


#define DECL_CHILD_RTTI( class_name )								\
public:																\
	static  const tdk::util::rtti  s_rtti;							\
	virtual const tdk::util::rtti* rtti( void ){					\
		return &class_name::s_rtti;									\
	}																\

#define IMPL_ROOT_RTTI( class_name )				const tdk::util::rtti class_name::s_rtti( #class_name , nullptr );
#define IMPL_CHILD_RTTI( class_name , parent_name ) const tdk::util::rtti class_name::s_rtti( #class_name , &parent_name::s_rtti );


/*

namespace no_virtal {

struct parent {
	DECL_ROOT_RTTI_NO_VIRTUAL;											 
public:
	parent( void ) {
		IMPL_RTTI_CTOR_NO_VIRTUAL( parent );
	}
};

IMPL_ROOT_RTTI( parent );

struct child : public parent {
	DECL_CHILD_RTTI_NO_VIRTUAL;

	child( void ) {
		IMPL_RTTI_CTOR_NO_VIRTUAL( child );
	}
};

IMPL_CHILD_RTTI( child , parent );


struct child2 : public child {
	DECL_CHILD_RTTI_NO_VIRTUAL;

	child2( void ) {
		IMPL_RTTI_CTOR_NO_VIRTUAL( child2 );
	}
};

IMPL_CHILD_RTTI( child2 , child );

}

TEST( rtti_test , no_virtual ) {
	no_virtal::parent* root = new no_virtal::parent;
	no_virtal::child* c1 = new no_virtal::child;
	no_virtal::child2* c2 = new no_virtal::child2;


	ASSERT_FALSE( root->is_kind_of<no_virtal::child>());
	ASSERT_FALSE( root->is_kind_of<no_virtal::child2>());
	std::cout << tdk::util::rtti::get<no_virtal::parent>( root )->name() << std::endl;
	delete root;

	root = c1;


	ASSERT_TRUE(  root->is_kind_of<no_virtal::child>());
	ASSERT_FALSE( root->is_kind_of<no_virtal::child2>());
	std::cout << tdk::util::rtti::get<no_virtal::parent>( root )->name() << std::endl;
	delete c1;

	root = c2;


	ASSERT_TRUE( root->is_kind_of<no_virtal::child>());
	ASSERT_TRUE( root->is_kind_of<no_virtal::child2>());
	std::cout << tdk::util::rtti::get<no_virtal::parent>( root )->name() << std::endl;

	delete c2;
}

namespace normal {
class parent {
	DECL_ROOT_RTTI;											 
public:
	parent( void ) {
		
	}

	virtual ~parent( void ) {

	}
};

IMPL_ROOT_RTTI( parent );

struct child : public parent {
	DECL_CHILD_RTTI;

	child( void ) {

	}

	virtual ~child( void ) {

	}
};

IMPL_CHILD_RTTI( child , parent );


struct child2 : public child {
	DECL_CHILD_RTTI;

	child2( void ) {

	}

	virtual ~child2( void ) {

	}
};

IMPL_CHILD_RTTI( child2 , child );

}

TEST( rtti_test , normal ) {
	normal::parent* root = new normal::parent;
	normal::child* c1 = new normal::child;
	normal::child2* c2 = new normal::child2;


	ASSERT_FALSE( root->is_kind_of<normal::child>());
	ASSERT_FALSE( root->is_kind_of<normal::child2>());
	delete root;

	root = c1;

	ASSERT_TRUE( root->is_kind_of<normal::child>());
	ASSERT_FALSE( root->is_kind_of<normal::child2>());

	delete c1;

	root = c2;


	ASSERT_TRUE( root->is_kind_of<normal::child>());
	ASSERT_TRUE( root->is_kind_of<normal::child2>());
}
*/

}}

#endif