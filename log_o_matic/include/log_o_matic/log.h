#ifndef INCLUDED_kapaga_log_HPP
#define INCLUDED_kapaga_log_HPP

#include "log_util.h"
#include "database_entry.h"
#include "database.h"
#include <sys/time.h>
#include <iostream>
#include <string>

namespace kapaga {

/**
 * This is a time logger which logs the the scope it is constructed in.
 * There are different options how the data is been logged.
 * Default when just created with log::log()
 * Output to std::cout: 23.42 seconds
 *
 * Addition you can be specify a tag and a ouput stream (want a file stream?).
 * Sample output:
 * [TAG]: 3.55778 seconds

 * The last option is by creating the log with log::log(database, "TAG").
 * Here the log information will not been put in a stream, but in a database
 * (type log::database).
 * @author Jens Breitbart
 */
class log {
public:
	/**
	 * Generates a log :-)
	 * @param _tag a tag specifying the current section
	 * @param _output_stream stream the output will be written to
	 */
	log(const char* _tag, std::ostream &_output_stream=std::cerr);
	log(const std::string& _tag=std::string(), std::ostream &_output_stream=std::cerr);

	/**
	 * Generates a log :-)
	 * @param _db the database
	 */
	log(const char* _tag, database &_db);
	log(const std::string& _tag, database &_db);


	virtual ~log();

	/**
	 * Writes the log @a _log to @a outstr.
	 */
	template< typename charT, class traits >
		friend
		std::basic_ostream< charT, traits >&
		operator<< ( std::basic_ostream< charT, traits >& outstr, log const& _log ) {
			timeval _end;
			gettimeofday(&_end, 0);
			const double _duration = timediff( _log.start_, _end);

			outstr << database_entry (_log.tag_, _duration);

			return outstr;
		}

private:
	/**
	 * Time the logging starts
	 */
	timeval start_;

	/**
	 * Tag used to specify the current log, will be written to the stream
	 */
	const std::string tag_;

	/**
	 * If true data is just logged in the database
	 */
	database *db_;

	/**
	 * The stream we write the output to
	 */
	std::ostream *output_stream_;

	/**
	 * Initialisation all internal data, called by constructors
	 * @param _tag
	 * @param _output_stream
	 * @param _db
	 */
	void initialisation(std::ostream *_output_stream, database *_db);
};

}

#endif
