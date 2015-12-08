#include "log_o_matic/database_entry.h"
#include <string>

#include <iostream>

namespace kapaga {

database_entry::database_entry(const std::string& _tag, const double _duration ) : tag_(_tag), duration_(_duration) {
}

database_entry::database_entry(const database_entry &cpy) : tag_(cpy.tag_), duration_(cpy.duration_) {
}

database_entry::~database_entry() {
}

bool database_entry::operator< (const database_entry &cmp) const {
	return tag_ < cmp.tag_;
}

database_entry& database_entry::operator+= (const database_entry &rhs) {
	if (tag_ != rhs.tag_) {
		throw "log_o_matic error: try to add two different tags";
	}

	duration_ += rhs.duration_;

	return *this;
}

}


