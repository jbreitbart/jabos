#include <memory>
#include <initializer_list>
#include <iostream>
#include <cassert>
#include <map>
#include <iterator>
#include <algorithm>
#include <limits>

struct capacity {
	// int _cores;
	int _hyperthreads;

	bool enough(int hyperthreads /*, int cores*/) { return /*_cores > cores &&*/ _hyperthreads > hyperthreads; }
};

struct work_tree_node {
	std::unique_ptr<work_tree_node[]> _children;
	work_tree_node *const _parent;
	capacity _cap;
	std::map<int, int> _processes;
	int _number_of_children;

	work_tree_node() : _parent(nullptr) {}

	work_tree_node(work_tree_node &) = delete;

	template <typename T> work_tree_node(const std::initializer_list<T> &list) : work_tree_node(list, nullptr, 1) {}
	~work_tree_node() {}

	void print() const {
		for (auto it = _processes.begin(); it != _processes.end(); ++it) {
			std::cout << "Key: " << it->first << " Values: " << it->second << std::endl;
		}

		for (int i = 0; i < _number_of_children; ++i) {
			std::cout << "child " << i << std::endl;
			_children[i].print();
		}
	}

	void scatter(int id, int hyperthreads /*, int cores*/) {
		work_tree_node *current = this;
		while (current->_children != nullptr) {
			// check if current has already an entry for our id
			if (current->_processes.count(id) == 0) {
				current->_processes.insert(std::pair<int, int>(id, 0));
			}

			// find child with minimum number of processes with our id
			int min_v = std::numeric_limits<int>::max();
			int min_id = 0;
			for (int i = 0; i < current->_number_of_children; ++i) {
				int count = current->_children[i]._processes.count(id);
				if (min_v > count && current->_cap.enough(hyperthreads /*, cores*/)) {
					min_v = count;
					min_id = i;
				}
			}
			current = &(current->_children[min_id]);
		}

		// ok, current now points to a children
		current->reserve(id, hyperthreads /*, cores*/);
	}

	void compact(int id, int hyperthreads /*, int cores*/) {}

	void reserve(int id, int hyperthreads) {
		_processes.insert(std::pair<int, int>(id, hyperthreads));
		_cap._hyperthreads -= hyperthreads;

		if (_parent != nullptr) _parent->reserve(id, hyperthreads);
	}

  private:
	template <typename T>
	work_tree_node(const std::initializer_list<T> &list, work_tree_node *const parent, const int level)
		: _parent(parent), _number_of_children(0) {
		// compute capacity
		_cap = {1};
		int temp = list.size() - 1;
		for (auto it = std::crbegin(list) + 1; it != std::crend(list); ++it) {
			if (level > temp) {
				break;
			}
			--temp;
			_cap._hyperthreads *= *(it - 1);
			//_cap._cores = _cap._hyperthreads / 2;
		}

		// end recursion if we are at the end of the list
		if (!(level < list.size())) return;

		int count = 0;
		// get the number of elements required in this level
		for (auto i = list.begin(); i != list.end(); ++i) {
			if (count == level) {
				count = *i;
				break;
			}
			++count;
		}
		_number_of_children = count;
		// std::cout << "count " << count << std::endl;

		_children = std::unique_ptr<work_tree_node[]>(new work_tree_node[count]);
		// create the work_tree_node in place
		for (int i = 0; i < count; ++i) {
			new (&_children[i]) work_tree_node(list, this, level + 1);
		}
	}
};

class work_tree {
  private:
	work_tree_node root;

  public:
	work_tree() = delete;
	template <typename T> work_tree(std::initializer_list<T> list) : root(list) {}

	void scatter(int id, int hyperthreads) { root.scatter(id, hyperthreads); }

	void compact(int id, int hyperthreads) { compact(id, hyperthreads); }

	void print() const { root.print(); }
};

int main(int argc, char const *argv[]) {
	work_tree asd({1, 2, 4, 2});

	asd.scatter(0, 2);
	asd.print();
	return 0;
}
