#include <iostream>
#include <cassert>

extern "C" {
#include "correction_tree.h"
}

using namespace std;

int main(int argc, char *argv[]) {
	jCC jccs[10];
	cor_node *n1 = process_call(nullptr, &jccs[0]);
	cor_node *n2 = process_call(n1, &jccs[1]);
	cor_node *n3 = process_return(n2, &jccs[0]);
	cor_node *n4 = process_return(n3, &jccs[2]);
	assert(n3 == n1);
	cout << n1->_cache_line_counter << endl;
}
