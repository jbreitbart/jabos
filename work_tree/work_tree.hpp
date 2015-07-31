#pragma once

#include "capacity.hpp"
#include "work_tree_node.hpp"

class work_tree {
  private:
	work_tree_node root;

  public:
	work_tree() = delete;
	template <typename T> work_tree(std::initializer_list<T> list) : root(list) {}

	void scatter(int key, int hyperthreads) { root.scatter(key, hyperthreads); }

	void compact(int key, int hyperthreads) { root.compact(key, hyperthreads); }

	void remove_all(int key) { root.remove_all(key); }
};
