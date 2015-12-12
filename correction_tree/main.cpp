#include <iostream>
#include <cassert>

extern "C" {
#include "correction_tree.h"
}

using namespace std;

int main(int argc, char *argv[]) {
	cor_node *n1 = process_call(nullptr, nullptr);
	cor_node *n2 = process_call(n1, nullptr);
	cor_node *n3 = process_return(n2, nullptr);
	cor_node *n4 = process_return(n3, nullptr);
	assert(n3 == n1);
	cout << n1->_cache_line_counter << endl;
}
