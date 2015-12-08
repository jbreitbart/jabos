#include "log_o_matic/database.h"

#include <vector>
#include <iostream>
#include <algorithm>

namespace kapaga {

database::database() {
}

database::database(const database &_db) : entries_(_db.entries_) {
}

database::~database() {
	typedef std::vector< database_entry >::size_type size_type;
	size_type db_size = entries_.size();
	for ( size_type i=0; i < db_size; ++i ) {
		std::cout << entries_[i];
	}
}


void database::push_back(const database_entry &_entry) {
	entries_.push_back(_entry);
}

void database::clear() {
	entries_.clear();
}

void database::reduce() {
	std::sort (entries_.begin(), entries_.end());

	std::vector<database_entry> new_entries;

	typedef std::vector<database_entry>::iterator iterator;
	typedef std::pair<iterator, iterator> iter_pair;

	iterator comp = entries_.begin();
	while (comp != entries_.end()) {
		const iter_pair p = std::equal_range(comp, entries_.end(), *comp);
		// *comp is always found, so (p.first != p.second) is always true
		
		new_entries.push_back (database_entry (*p.first) );

		const int last_index = new_entries.size()-1;
		for (iterator i=p.first+1; i<p.second; ++i) {
			new_entries[last_index] += *i;
		}

		comp = p.second;
	}

	entries_ = new_entries;
}

}
