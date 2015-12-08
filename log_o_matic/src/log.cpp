#include "log_o_matic/log.h"
#include "log_o_matic/log_util.h"
#include "log_o_matic/database.h"
#include "log_o_matic/database_entry.h"

#include <sys/time.h>
#include <iostream>
#include <string>


namespace kapaga {

log::log(const char* _tag, std::ostream &_output_stream) : tag_(_tag) {
	initialisation( &_output_stream, 0);
}

log::log(const char* _tag, database &_db) : tag_(_tag) {
	initialisation( 0, &_db);
}

log::log(const std::string& _tag, std::ostream &_output_stream) : tag_(_tag) {
	initialisation( &_output_stream, 0);
}

log::log(const std::string& _tag, database &_db) : tag_(_tag){
	initialisation( 0, &_db);
}


log::~log() {
	timeval _end;
	gettimeofday(&_end, 0);
	const double _duration = timediff( start_, _end);

	database_entry _temp(tag_, _duration);

	if (!db_) {
		*output_stream_ << _temp;
	} else {
		db_->push_back(_temp);
	}
}

void log::initialisation(std::ostream *_output_stream, database *_db) {
	output_stream_=_output_stream;
	db_=_db;
	gettimeofday(&start_, 0);
}

}
