#ifndef __tdk_geo_h__
#define __tdk_geo_h__

namespace tdk {

struct size {
	int width;
	int height;
};

struct rect {
	template < typename A , typename B >
	void set( A& a , const B& b ){
		a.left = b.left;	a.top = b.top; a.right = b.right; a.bottom = b.bottom;
	}

	rect( const RECT& r ) { set( *this , r); }
	rect( const rect& r ) { set( *this , r); }
	rect& operator=( const RECT& r ) { set( *this , r); return *this; }
	rect& operator=( const rect& r ) { set( *this , r); return *this; }

	explicit rect( const int l = 0 , const int t = 0 , const int r = 0 , const int b = 0 ) {
		left = l; top = t; right = r; bottom = b;
	}

	RECT to_RECT( void ) {
		RECT r;
		set( r , *this );
		return r;
	}

	int width( void ) {
		return max( right , left ) - min( right , left );
	}

	int height( void ) {
		return max( top , bottom ) - min( top , bottom );
	}


	int left;
	int top;
	int right;
	int bottom;
};

struct point {
	int x;
	int y;
};


}

#endif