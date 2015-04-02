#include <iostream>
#include <unordered_map>
#include <ctime>

#ifdef LESSDIV
// Specialization of unordered_map to use masking for bucket calculation

// Compute mask using in _Mask_range_hashing
// mask := (smallest power of two greater or equal than __den) - 1
template <std::size_t SHIFT>
static inline std::size_t
_Mask_range_hashing_compute_mask(std::size_t n)
{
    n = _Mask_range_hashing_compute_mask<SHIFT/2>(n);
    n |= n >> SHIFT;
    return n;
}

// End recursion for compute_mask used in _Mask_range_hashing
template<>
inline std::size_t
_Mask_range_hashing_compute_mask<1>(std::size_t n)
{
    n |= n >> 1;
    return n;
}

// Specialization of compute_mask used in _Mask_range_hashing for 64bit systems
template<>
inline std::size_t
_Mask_range_hashing_compute_mask<32>(std::size_t n)
{
    n |= n >> 1;
    n |= n >> 2;
    n |= n >> 4;
    n |= n >> 8;
    n |= n >> 16;
    n |= n >> 32;
    return n;
}

/// Range hashing function using masking to fold a large number into the
/// range [0, N). We use a mask larger than the number of buckets. In case
/// result of the masking is not a valid bucket number, we fall back to modulo
/// as it is used in _Mod_range_hashing.
struct _Mask_range_hashing
{
    typedef std::size_t first_argument_type;
    typedef std::size_t second_argument_type;
    typedef std::size_t result_type;

    // mutable because it must be changed within operator()
    mutable std::size_t _M_mask;

    _Mask_range_hashing() : _M_mask(1) {}

    result_type
    operator()(first_argument_type __num,
           second_argument_type __den) const noexcept
    {
        if (_M_mask < __den) {
            _M_mask = _Mask_range_hashing_compute_mask<sizeof(std::size_t)*8/2>(__den - 1);
        }
        const std::size_t probe = (__num & _M_mask);
        // probe could be greater than the number of buckets since mask can be
        // greater. Do a modulo operation only if that is the case
        const std::size_t b = (probe < __den) ? probe : (__num % __den);

        return b;
    }
};

namespace std {
template <typename _Alloc, typename _ExtractKey, typename _Equal, typename _H1, typename _Hash, typename _RehashPolicy,
		  typename _Traits>
class _Hashtable<int, std::pair<const int, int>, _Alloc, _ExtractKey, _Equal, _H1, __detail::_Mod_range_hashing, _Hash,
				 _RehashPolicy, _Traits>
	: public _Hashtable<int, std::pair<const int, int>, _Alloc, _ExtractKey, _Equal, _H1, _Mask_range_hashing, _Hash,
						_RehashPolicy, _Traits> {
  public:
	using myBase = _Hashtable<int, std::pair<const int, int>, _Alloc, _ExtractKey, _Equal, _H1, _Mask_range_hashing,
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
