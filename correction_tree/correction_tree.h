#ifndef correction_tree_h
#define correction_tree_h

// TODO extern C for c++

// TODO change for valgrind
typedef void jCC;
#define MALLOC malloc
#define REALLOC realloc
#define FREE free

struct cor_node {
	unsigned int _cache_line_counter; // TODO max size = L1 + L2 cache line count
	unsigned int _counters[4];

	jCC *_jCC;

	struct cor_node *_caller;
	struct cor_node **_callees;
	unsigned int _callees_size;
	unsigned int _callees_capacity;
};

struct cor_node *process_call(struct cor_node *node, jCC *ptr);

struct cor_node *process_return(struct cor_node *node, jCC *ptr);

void add_cacheline(struct cor_node *node);

void evict_cache_line(struct cor_node *node, struct cor_node *cache_line_node, unsigned char level, unsigned int acc,
					  unsigned int loss);

#endif /* end of include guard: correction_tree_h */
