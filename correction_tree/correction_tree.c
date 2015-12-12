#include "correction_tree.h"

#include <stdlib.h>
#include <assert.h>

static inline void init_cor_node(struct cor_node *node) {
	node->_cache_line_counter = 0;
	node->_counters[0] = 0;
	node->_counters[1] = 0;
	node->_counters[2] = 0;
	node->_counters[3] = 0;
	node->_caller = 0;
	node->_callees = 0;
	node->_callees_size = 0;
	node->_callees_capacity = 0;
	node->_jCC = 0;
}

struct cor_node *process_call(struct cor_node *node, jCC *jcc) {
	// start a new tree?
	if (node == 0) {
		struct cor_node *returnee = 0;
		returnee = MALLOC(sizeof(struct cor_node));
		assert(returnee != 0);

		init_cor_node(returnee);
		returnee->_jCC = jcc;

		return returnee;
	}

	// check if there is already an entry with the jcc in our callees
	for (int i = 0; i < node->_callees_size; ++i) {
		if (node->_callees[i]->_jCC == jcc) return node->_callees[i];
	}

	// no new tree, we add a new callee and return it
	const int callees_free = node->_callees_capacity - node->_callees_size;
	assert(callees_free >= 0);

	// any room left in the node for a new callee?
	if (callees_free == 0) {
		// TODO magic numer. Ask if there is any domain knowledge for the number
		const unsigned int magic_number = 2;
		struct cor_node **temp =
			REALLOC(node->_callees, (node->_callees_capacity + magic_number) * sizeof(struct cor_node *));
		assert(temp != 0);
		node->_callees = temp;
		for (int i = 0; i < magic_number; ++i) {
			node->_callees[i + node->_callees_capacity] = 0;
		}
		node->_callees_capacity += 2;
	}
	const unsigned int i = node->_callees_size++;

	if (node->_callees[i] == 0) node->_callees[i] = MALLOC(sizeof(struct cor_node));
	assert(node->_callees[i] != 0);
	init_cor_node(node->_callees[i]);
	node->_callees[i]->_jCC = jcc;
	node->_callees[i]->_caller = node;

	return node->_callees[i];
}

struct cor_node *process_return(struct cor_node *node, jCC *jcc) {
	// return above our tree root
	if (node->_caller == 0) {
		node->_caller = MALLOC(sizeof(struct cor_node));
		assert(node->_caller == 0);
		node->_caller->_jCC = jcc;
	}
	assert(node->_caller->_jCC == jcc);
	// TODO check if we have no callees and no cache lines
	//      if that is true we should remove ourself
	return node->_caller;
}

void add_cacheline(struct cor_node *node) { ++node->_cache_line_counter; }

void evict_cache_line(struct cor_node *node, struct cor_node *cache_line_node, unsigned char level, unsigned int acc,
					  unsigned int loss) {
	assert(cache_line_node->_cache_line_counter >= 1);
	assert(level == 1 || level == 2);

	--cache_line_node->_cache_line_counter;
	if (level == 1) {
		cache_line_node->_counters[0] += acc;
		cache_line_node->_counters[1] += loss;
	}
	if (level == 2) {
		cache_line_node->_counters[2] += acc;
		cache_line_node->_counters[3] += loss;
	}

	// was that the last cache line?
	if (cache_line_node->_cache_line_counter == 0) {
		// jcc->cost[:] = counter[:]
		for (int i = 0; i < 4; ++i) {
			cache_line_node->_counters[i] = 0;
		}

		// remove the node if it is not the current one
		if (node != cache_line_node) {
			struct cor_node *p = cache_line_node->_caller;
			unsigned int id = p->_callees_size;
			for (int i = 0; i < p->_callees_size; ++i) {
				if (p->_callees[i] == cache_line_node) {
					id = i;
					break;
				}
			}
			assert(id != p->_callees_size);
			p->_callees[id] = p->_callees[p->_callees_size - 1];
			p->_callees[p->_callees_size - 1] = 0;
			// TODO delete node
			--p->_callees_size;
		}
	}
}
