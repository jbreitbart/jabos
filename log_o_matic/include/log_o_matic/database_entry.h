#ifndef INCLUDED_kapaga_database_entry_HPP
#define INCLUDED_kapaga_database_entry_HPP

// Include std::basic_ostream
#include <ostream>
#include <string>

namespace kapaga {

/**
 * A database entry :-)
 * @author Jens Breitbart
 */
class database_entry{
public:
	/**
	 * Generates the database entry with the given information
	 * @param _tag the tag of this entry
	 * @param _duration the duration of this entry
	 */
	database_entry(const std::string& _tag, const double _duration);

	/**
	 * Copy constructor
	 */
	database_entry(const database_entry &cpy);

	~database_entry();

	/**
	 * Writes the database @a db to @a outstr.
	 */
	template< typename charT, class traits >
		friend
		std::basic_ostream< charT, traits >&
		operator<< ( std::basic_ostream< charT, traits >& outstr,
					 database_entry const& db_entry ) {
			if (db_entry.tag_!="") {
				outstr << "[" << db_entry.tag_ << "]: ";
			}

			outstr << db_entry.duration_ << " seconds" << std::endl;
			return outstr;
		}

	/**
	 * Compares the tags
	 */
	bool operator< (const database_entry &cmp) const;

	/**
	 * Adds the two durations. Only possible if both tags are identical.
	 * @throws char[] "log_o_matic error: try to add two different tags"
	 */
	database_entry& operator+= (const database_entry &rhs);

private:
	std::string tag_;
	double duration_;
};

}

#endif
