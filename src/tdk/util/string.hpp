#ifndef __tdk_string_h__
#define __tdk_string_h__

#include <string>

namespace tdk {
namespace string {
namespace detail {

template < typename T_char >
struct length;

template <>
struct length< char > {
	static std::size_t value( char* msg ) {
		return strlen(msg);
	}
};

template <>
struct length< wchar_t > {
	static std::size_t value( wchar_t* msg ) {
		return wcslen(msg);
	}
};

}

std::string  wcs_to_mbs( const std::wstring& in );
std::wstring mbs_to_wcs( const std::string&  in );

std::string  mbs_to_utf_8( const std::string& in );
std::string  wcs_to_utf_8( const std::wstring& in );

std::wstring utf_8_to_wcs( const std::string& in );
std::string  utf_8_to_mbs( const std::string& in );


std::string& append_format( std::string& msg , const char* format , ... );
std::wstring& append_format( std::wstring& msg , const wchar_t* format , ... );	

template < typename string_type , typename out >
static void split( const string_type& message , const string_type& sep , out os ) {
	typename string_type::size_type msgEnd = message.length();
    typename string_type::size_type start  = 0;
    typename string_type::size_type stop   = 0;
        
    start = message.find_first_not_of( sep );
    while ( true ) {
        if ( start >= msgEnd ) break;

        stop = message.find_first_of(sep, start);
        if ( stop == string_type::npos ) 
            stop = msgEnd;

        *os = message.substr( start , stop - start );
        start = message.find_first_not_of( sep , stop + 1 );
		++os;
    }
}

template < typename string_type  >
static string_type replace( const string_type& message , const string_type& pattern , const string_type& replace ) {
    string_type out = message;
    typename string_type::size_type start  = 0;
    typename string_type::size_type stop   = 0;

    while ( true ) {
        start = out.find( pattern , stop );
        if ( start  == string_type::npos ) 
            break;
        stop = start + pattern.length();
        out.replace( out.begin() + start , out.begin() + stop , replace );
    }
    return out;
}

template < typename string_type >
static string_type ltrim( const string_type& msg , const string_type& trimStr ) {
	typename string_type::size_type start = msg.find_first_not_of( trimStr );
	if ( start != string_type::npos ) {
		return msg.substr( start , msg.length() - start );
	}
	return msg;
}

template < typename string_type >
static string_type rtrim( const string_type& msg , const string_type& trimStr ) {
	typename string_type::size_type end = msg.find_last_not_of( trimStr );
	if ( end != string_type::npos ) {
		return msg.substr( 0 , end + 1 );
	}
	return msg;
}
	
template < typename string_type >
static void trim( string_type& msg , const string_type& trimStr ) {
	typename string_type::size_type pos = msg.find_last_not_of( trimStr );
	if ( pos != string_type::npos ) {
		msg.erase( pos + 1 );
	}

	pos = msg.find_first_not_of( trimStr );
	if ( pos != string_type::npos ) {
		msg.erase( 0 , pos );
	}
}

template < typename string_type >
static string_type to_upper( const string_type& msg ) {
	string_type out;
	out.reserve( msg.length() );
	for ( typename string_type::size_type idx = 0 ; 
			idx < msg.length() ; 
			++idx ) {

		if ( msg[idx] < 0xff && isalpha( msg[idx] ) && islower( msg[idx] ) ) {
			out.push_back( _toupper(msg[idx]) );
		} else {
			out.push_back( msg[idx] );
		}
	}
	return out;
}

template < typename string_type >
static string_type to_lower( const string_type& msg ) {
	string_type out;
	out.reserve( msg.length() );
	for ( typename string_type::size_type idx = 0 ; 
			idx < msg.length() ; 
			++idx ) {
		if ( msg[idx] < 0xff && isalpha( msg[idx]) &&  isupper( msg[idx] ) ) {
			out.push_back( _tolower(msg[idx]) );
		} else {
			out.push_back( msg[idx] );
		}
	}
	return out;
}

template < typename T_char >
static T_char* ltrim( T_char* msg , T_char ch =' ' ){
	if ( message != nullptr ) {
        T_char* start      = message;
        T_char* ltrimStart = message;

        while ( *ltrimStart == ch )
            ++ltrimStart;

        if ( ltrimStart == start )
            return message;
        
        while ( *ltrimStart ) {
            *(start++) = *(ltrimStart++);
        }
        *start = '\0';
    }
    return message;
}

template < typename T_char >
static T_char* rtrim( T_char* msg , T_char ch = ' ' ){
	if ( message != nullptr ) {
		size_t len = detail::length<T_char>::value( message );
        T_char* rtrimEnd = message + len;
        while ( *(--rtrimEnd) == ch );
        *(++rtrimEnd) = '\0';
    }
    return message;
}

static bool is_hangul( wchar_t ch );

class hangul {
public:
	hangul( const wchar_t cho , const wchar_t jung , const wchar_t jong );
	~hangul( void );

	wchar_t cho_sung() const ;
	wchar_t jung_sung() const ;
	wchar_t jong_sung() const ;
private:
	wchar_t _ChoSung;
	wchar_t _JungSung;
	wchar_t _JongSung;
};

static hangul extract_hangul( wchar_t ch );

}}

#endif