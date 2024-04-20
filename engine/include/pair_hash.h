#ifndef PAIR_HASH_H
#define PAIR_HASH_H

#include <functional>
#include <utility>

namespace roingine {
	struct PairHash final {
		template<class T1, class T2>
		std::size_t operator()(std::pair<T1, T2> const &pair) const {
			return std::hash<T1>()(pair.first) ^ std::hash<T2>()(pair.second);
		}
	};
}// namespace roingine

#endif// PAIR_HASH_H
