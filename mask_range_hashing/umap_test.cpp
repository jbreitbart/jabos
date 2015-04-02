#include <iostream>
#include <unordered_map>
#include <ctime>

#ifdef LESSDIV
// Specialization of unordered_map to use masking for bucket calculation
struct _Mod_myrange_hashing {
	typedef std::size_t first_argument_type;
	typedef std::size_t second_argument_type;
	typedef std::size_t result_type;

	static std::size_t mask;

	_Mod_myrange_hashing() {}
	_Mod_myrange_hashing(const std::__detail::_Mod_range_hashing &/*temp*/) {}

	result_type operator()(first_argument_type __num, second_argument_type __den) const noexcept {
		if (mask < __den) {
			// Compute mask
			// mask := (smallest power of two greater or equal than __den) - 1
			std::size_t n = __den - 1;
			n |= n >> 1;
			n |= n >> 2;
			n |= n >> 4;
			n |= n >> 8;
			n |= n >> 16;
			n |= n >> 32;
			mask = n;
		}
		const std::size_t probe = (__num & mask);
        // probe could be greater than the number of buckets since mask can be
        // greater. Do a modulo operation only if that is the case
		const std::size_t b = (probe < __den) ? probe : (__num % __den);

		return b;
	}
};

std::size_t _Mod_myrange_hashing::mask = 1;

namespace std {
template <typename _Alloc, typename _ExtractKey, typename _Equal, typename _H1, typename _Hash, typename _RehashPolicy,
		  typename _Traits>
class _Hashtable<int, std::pair<const int, int>, _Alloc, _ExtractKey, _Equal, _H1, __detail::_Mod_range_hashing, _Hash,
				 _RehashPolicy, _Traits>
	: public _Hashtable<int, std::pair<const int, int>, _Alloc, _ExtractKey, _Equal, _H1, _Mod_myrange_hashing, _Hash,
						_RehashPolicy, _Traits> {
  public:
	using myBase = _Hashtable<int, std::pair<const int, int>, _Alloc, _ExtractKey, _Equal, _H1, _Mod_myrange_hashing,
							  _Hash, _RehashPolicy, _Traits>;

	using myBase::_Hashtable;
	using mySizeType = typename myBase::size_type;
};
}
#endif

constexpr int INSERTS = 50000000;
constexpr int FINDS = 500000000;

int main() {
	using namespace std;
	unordered_map<int, int> umap;

	clock_t begin = clock();
	for (int i = 0; i < INSERTS; ++i) umap.emplace(i, 0);
	clock_t end = clock();
	cout << "runtime(s) emplace = " << double(end - begin) / CLOCKS_PER_SEC << endl;

	begin = clock();
	for (int i = 0; i < FINDS; ++i) {
		(void)umap.find(i);
	}
	end = clock();
	cout << "runtime(s) find = " << double(end - begin) / CLOCKS_PER_SEC << endl;

	return 0;
}
