#pragma once

#include "capacity.hpp"

struct work_tree_node {
	// Increased for every root node
	static int id_counter;

	// Array of children
	std::unique_ptr<work_tree_node[]> _children;

	// Stores a application key and how many ressources are used
	std::map<int, int> _processes;

	// The available ressources
	capacity _cap;

	// Pointer to the parent
	work_tree_node *const _parent = nullptr;
	int _number_of_children = 0;
	int _id = 0;
	int _level = 0;

	work_tree_node() : _parent(nullptr) {}

	work_tree_node(work_tree_node &) = delete;

	template <typename T> work_tree_node(const std::initializer_list<T> &list) : work_tree_node(list, nullptr, 1) {}
	~work_tree_node() {}

	void scatter(int key, int hyperthreads) {
		if (!_cap.enough(hyperthreads)) throw "kein platz mehr :)";

		// find child with minimum number of processes with our key
		int min_v = std::numeric_limits<int>::max();
		int min_id = -1;
		for (int i = 0; i < _number_of_children; ++i) {
			int count = _children[i]._processes.count(key);
			if (_children[i]._cap.enough(hyperthreads)) {
				if (count < min_v) {
					min_v = count;
					min_id = i;
				}
			}
		}

		// true if there are no children of if the capacity of the children is not big enough
		if (min_id == -1) {
			std::cout << "allocating key " << key << " on " << _id << "@" << _level << std::endl;
			reserve(key, hyperthreads);
			return;
		}

		_children[min_id].scatter(key, hyperthreads);
	}

	void compact(int key, int hyperthreads) {
		if (!_cap.enough(hyperthreads)) throw "kein platz mehr :)";

		// find child with maximum number of processes with our key
		int min_v = -1;
		int min_id = -1;
		for (int i = 0; i < _number_of_children; ++i) {
			int count = _children[i]._processes.count(key);
			// std::cout << "checking " << _children[i]._id << "@" << _children[i]._level << ": "
			//		  << count << " - " << _children[i]._cap  << " - " << hyperthreads << std::endl;
			if (_children[i]._cap.enough(hyperthreads)) {
				if (count > min_v) {
					min_v = count;
					min_id = i;
				}
			}
		}

		// true if there are no children of if the capacity of the children is not big enough
		if (min_id == -1) {
			std::cout << "allocating key " << key << " on " << _id << "@" << _level << std::endl;
			reserve(key, hyperthreads);
			return;
		}

		_children[min_id].compact(key, hyperthreads);
	}

	void reserve(int key, int hyperthreads) {
		auto temp = _processes.find(key);
		if (temp == _processes.end()) {
			_processes.insert(std::pair<int, int>(key, hyperthreads));
		} else {
			temp->second += hyperthreads;
		}
		_cap -= hyperthreads;
		// std::cout << "decreased cap on " << _id << "@" << _level << " by " << hyperthreads << " to " << _cap
		//		  << std::endl;

		if (_parent != nullptr) _parent->reserve(key, hyperthreads);
	}

	void remove_all(int key) {
		if (_processes.count(key) == 0) {
			// std::cout << "nothing at " << _id << "@" << _level << " - " << _cap << std::endl;
			return;
		}

		for (int i = 0; i < _number_of_children; ++i) _children[i].remove_all(key);

		auto it = _processes.find(key);
		_cap += it->second;
		_processes.erase(it);
		// std::cout << "increased cap on " << _id << "@" << _level << " by " << it->second << " to " << _cap <<
		// std::endl;
	}

  private:
	template <typename T>
	work_tree_node(const std::initializer_list<T> &list, work_tree_node *const parent, const int level)
		: _parent(parent), _level(level) {
		// compute capacity
		_cap = 1;
		int temp = list.size() - 1;
		for (auto it = std::crbegin(list) + 1; it != std::crend(list); ++it) {
			if (level > temp) {
				break;
			}
			--temp;
			_cap *= *(it - 1);
		}

		_id = id_counter;
		// std::cout << "Created node with ID " << _id << " @ level " << level << std::endl;
		// end recursion if we are at the end of the list
		if (!(level < list.size())) {
			++id_counter;
			return;
		}

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
