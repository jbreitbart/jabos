#include <vector>
#include <cstdlib>
#include <limits>
#include <cstddef>

extern "C" {
#include "pub_tool_basics.h"
#include "pub_tool_debuginfo.h"
#include "pub_tool_libcbase.h"
#include "pub_tool_libcassert.h"
#include "pub_tool_libcprint.h"
#include "pub_tool_machine.h"
#include "pub_tool_mallocfree.h"
#include "pub_tool_options.h"
#include "pub_tool_tooliface.h"
#include "pub_tool_xarray.h"
#include "pub_tool_clientstate.h"
#include "pub_tool_machine.h" // VG_(fnptr_to_fnentry)
}

void std::__throw_length_error(char const *) { abort(); }

/**
 * This is an allocator using __libc_* functions.
 * It can be used in the malloc() function.
 */
template <class T> struct libc_allocator {
	using size_type = std::size_t;
	using difference_type = std::ptrdiff_t;
	using pointer = T *;
	using const_pointer = const T *;
	using reference = T &;
	using const_reference = const T &;
	using value_type = T;

	template <class U> struct rebind { typedef libc_allocator<U> other; };
	libc_allocator() throw() {}
	libc_allocator(const libc_allocator &) throw() {}

	template <class U> libc_allocator(const libc_allocator<U> &) throw() {}

	~libc_allocator() throw() {}

	pointer address(reference x) const { return &x; }
	const_pointer address(const_reference x) const { return &x; }

	pointer allocate(size_type s, void const * = 0) {
		if (s == 0) return nullptr;
		pointer temp = (pointer)VG_(malloc)("test_malloc", s * sizeof(T));
		// VG_(printf)("malloc! %p - %lu \n", temp, s);
		if (temp == nullptr) VG_(tool_panic)("alloc panic");
		return temp;
	}

	void deallocate(pointer p, size_type) {
		// VG_(printf)("free! %p \n", p);
		VG_(free)(p);
	}

	size_type max_size() const throw() { return std::numeric_limits<size_t>::max() / sizeof(T); }

	void construct(pointer p, const T &val) { new ((void *)p) T(val); }

	void destroy(pointer p) { p->~T(); }
};

static std::vector<int, libc_allocator<int>> *a = nullptr;
extern "C" int dummy() {
	if (a == nullptr) {
		a = (std::vector<int, libc_allocator<int>> *)VG_(malloc)("1", sizeof(std::vector<int, libc_allocator<int>>));
		a = new (a) std::vector<int, libc_allocator<int>>();
	}
	a->push_back(0);

	return (*a)[0];
}
