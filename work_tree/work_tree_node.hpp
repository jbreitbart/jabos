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
		work_tree_node *current = this;
		while (current->_children != nullptr) {
			// check if current has already an entry for our key
			if (current->_processes.count(key) == 0) {
				current->_processes.insert(std::pair<int, int>(key, 0));
			}

			// find child with minimum number of processes with our key
			int min_v = std::numeric_limits<int>::max();
			int min_id = -1;
			for (int i = 0; i < current->_number_of_children; ++i) {
				int count = current->_children[i]._processes.count(key);
				if (current->_children[i]._cap.enough(hyperthreads)) {
					if (count < min_v) {
						min_v = count;
						min_id = i;
					}
				}
			}
			if (min_id == -1) {
				// use the complete node
				break;
			}

			current = &(current->_children[min_id]);
		}

		// ok, current now points to a children
		std::cout << "allocating key " << key << " on " << current->_id << "@" << _level << std::endl;
		current->reserve(key, hyperthreads);
	}

	void compact(int key, int hyperthreads) {
		if (!_cap.enough(hyperthreads)) throw "kein platz mehr :)";
		work_tree_node *current = this;
		while (current->_children != nullptr) {
			// check if current has already an entry for our key
			if (current->_processes.count(key) == 0) {
				current->_processes.insert(std::pair<int, int>(key, 0));
			}

			// find child with minimum number of processes with our key
			int min_v = -1;
			int min_id = -1;
			for (int i = 0; i < current->_number_of_children; ++i) {
				int count = current->_children[i]._processes.count(key);
				// std::cout << "checking " << current->_children[i]._id << "@" << current->_children[i]._level << ": "
				//		  << count << " - " << current->_children[i]._cap  << " - " << hyperthreads << std::endl;
				if (current->_children[i]._cap.enough(hyperthreads)) {
					if (count > min_v) {
						min_v = count;
						min_id = i;
					}
				}
			}
			if (min_id == -1) {
				// use the complete node
				break;
			}

			current = &(current->_children[min_id]);
		}

		// ok, current now points to a children
		std::cout << "allocating key " << key << " on " << current->_id << "@" << _level << std::endl;
		current->reserve(key, hyperthreads);
	}

	void reserve(int key, int hyperthreads) {
		auto temp = _processes.find(key);
		temp->second += hyperthreads;
		_cap -= hyperthreads;
		// std::cout << "decreased cap on " << _id << "@" << _level << " by " << hyperthreads << " to " << _cap
		//		  << std::endl;

		if (_parent != nullptr) _parent->reserve(key, hyperthreads);
	}

	void remove_all(int key) {
		if (_processes.count(key) == 0) return;

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
