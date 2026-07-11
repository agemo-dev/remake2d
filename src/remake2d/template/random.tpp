#ifndef REMAKE2D_RANDOM_TPP_
#define REMAKE2D_RANDOM_TPP_

#include <remake2d/utility.hpp>

namespace rmk {

template<IsBasicType T> T Random::rand(const T& min, const T& max) noexcept {
	
	if constexpr (std::integral<T>) {
    	std::uniform_int_distribution<T> dist(min, max);
		return dist(m_gen);
		
	} else if constexpr (std::floating_point<T>) {
		std::uniform_real_distribution<T> dist(min, max);
		return dist(m_gen);
		
	} 

	return nil;
}


template<typename T>
T Random::choice(std::span<T> choices) noexcept {
    std::vector<T> vec(choices.begin(), choices.end());
    if(vec.empty()) return nil;
	
    std::uniform_int_distribution<u32> dist(0, vec.size() - 1);
    return vec[dist(m_gen)];
}

}//namespace rmk

#endif