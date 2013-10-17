/*
 * remote_time.hpp
 *
 *  Created on: 2013. 10. 17.
 *      Author: tk
 */

#ifndef REMOTE_TIME_HPP_
#define REMOTE_TIME_HPP_

#include <tdk/time/time_span.hpp>
#include <map>
namespace tdk {

// utc base
class remote_time {
public:
	remote_time();
	~remote_time();

	void register_time( const std::string& name , const tdk::time_span& diff);
	tdk::time_span diff( const std::string& name );
private:
	std::map< std::string , tdk::time_span > _remotes;
};

} /* namespace tdk */

#endif /* REMOTE_TIME_HPP_ */
