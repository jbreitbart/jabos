#include <iostream>
#include <cstdlib>
#include <string>
#include "log_o_matic/log.h"
#include "log_o_matic/database.h"

using namespace std;

/**
 * Just an simple example which shows how to use the log_o_matic.
 */

int main() {

	//we will need these two
	using kapaga::log;
	using kapaga::database;

	//a database to store some logs
	database _db;
	database _reduce_me;

	{
		//this is our log stored in the database
		log _db_log("db", _db);
		{
			//this log will be printed to cerr when its destructor is called
			log _log("Useless loop");

			for (int i=0; i<10; ++i) {
				log _x;
				log _xy("reducer", _reduce_me);
				sleep(1);
			}

			log ("alone in the dark", _reduce_me);

			cout << "NDT" << endl; //Na dann tschüss
		}

		//lets have a look at our db log, this will stream the current state of the log to cout
		cout << _db_log;
	}

	_reduce_me.reduce();

	cout << "*** database ***" << endl;

	// autmatically outputted when the databases destructor is called

	return EXIT_SUCCESS;
}
