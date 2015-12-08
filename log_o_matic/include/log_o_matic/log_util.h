#ifndef INCLUDED_kapaga_log_util_HPP
#define INCLUDED_kapaga_log_util_HPP

#include <sys/time.h>

/**
 * Collection of some function used in by the logger.
 * @author Jens Breitbart
 */

namespace kapaga {

/**
* Calculates the difference between two given timeval structs
* @param _start
* @param _end
* @return seconds elapsed between the start and the end
*/
inline double timediff(const timeval &_start, const timeval &_end) {
	return static_cast<double>(_end.tv_sec - _start.tv_sec) + (static_cast<double>(_end.tv_usec - _start.tv_usec) / 1000000.0);
}

}

#endif
