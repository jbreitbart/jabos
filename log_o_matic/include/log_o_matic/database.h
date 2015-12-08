#ifndef INCLUDED_kapaga_database_HPP
#define INCLUDED_kapaga_database_HPP

#include <vector>

// Include std::basic_ostream
#include <ostream>



#include "database_entry.h"

namespace kapaga {

/**
 * A database which stores all data from the logs.
 * @author Jens Breitbart
 */
class database {
public:
	database();
	database(const database &_db);
	~database();

	/**
	 * Adds an extry to the database
	 */
	void push_back(const database_entry &_entry);

	/**
	 * Writes the database @a db to @a outstr.
	 */
	template< typename charT, class traits >
		friend
		std::basic_ostream< charT, traits >&
		operator<< ( std::basic_ostream< charT, traits >& outstr,
					 database const& db ) {
			typedef std::vector< database_entry >::size_type size_type;
			size_type db_size = db.entries_.size();
			for ( size_type i=0; i < db_size; ++i ) {
				outstr << db.entries_[i];
			}

			return outstr;
		}

	/**
	 * Removes all entries from the databse
	 */
	void clear();

	/**
	 * Reduces the database based on the tags of the entries. Meaning
	 * DB before reduce:
	 * - ("Loop 1", 5sec), ("Loop 2", 5sec), ("Loop 1", 15sec)
	 * After reduce:
	 * - ("Loop 1", 20sec), ("Loop 2", 5sec)
	 */
	void reduce();

private:
	std::vector<database_entry> entries_;

};

}

#endif
