#include <memory>
#include <initializer_list>
#include <iostream>
#include <cassert>
#include <map>
#include <iterator>
#include <algorithm>
#include <limits>

#include "work_tree.hpp"

int work_tree_node::id_counter = 0;

int main(int argc, char const *argv[]) {
	work_tree asd({1, 2, 4, 2});

	asd.compact(0, 1);
	asd.compact(0, 2);

	asd.remove_all(0);

	asd.scatter(2, 1);
	asd.scatter(2, 2);

	asd.scatter(1, 1);

	return 0;
}
