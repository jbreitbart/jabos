#include <memory>
#include <initializer_list>
#include <iostream>

struct work_tree_node {
	std::unique_ptr<work_tree_node[]> _children;
	work_tree_node const *const _parent;
	int _level;

	template <typename T> work_tree_node(std::initializer_list<T> list) : work_tree_node(list, nullptr, 0) {
		std::cout << "Main constructor called" << std::endl;
	}
	~work_tree_node() { std::cout << "desctructor called - " << _level << std::endl; }

  private:
	template <typename T>
	work_tree_node(std::initializer_list<T> list, work_tree_node const *const parent, int level)
		: _parent(parent), _level(level) {
		std::cout << "constructor called on level " << level << std::endl;
		// end recursion
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

		// allocate raw memory with operator new[]
		work_tree_node *raw_mem = static_cast<work_tree_node *>(operator new[](count * sizeof(work_tree_node)));
		// create the work_tree_node in place
		for (int i = 0; i < count; ++i) {
			// new (&raw_mem[i]) work_tree_node(list, this, level + 1);
		}

		_children = std::unique_ptr<work_tree_node[]>(raw_mem);
	}
};

class work_tree {
  private:
	work_tree_node root;

  public:
	template <typename T> work_tree(std::initializer_list<T> list) : root(list) {}
};

int main(int argc, char const *argv[]) {
	work_tree asd({1, 1, 1});
	return 0;
}
