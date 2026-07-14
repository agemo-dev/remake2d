#ifndef REMAKE2D_RANDOM_
#define REMAKE2D_RANDOM_

#include <remake2d/color.hpp>
#include <remake2d/vector.hpp>
#include <remake2d/numeric.hpp>
#include <remake2d/concept.hpp>

#include <span>
#include <vector>
#include <chrono>
#include <random>
#include <string>
#include <concepts>
#include <stdlib.h>
#include <algorithm>

namespace rmk {

class Random {
private:
    u32             m_seed;
    std::mt19937    m_gen;

private:
    Random(void);
    Random(Random&&)                    = delete;
    Random(const Random&)               = delete;
    Random& operator=(Random&&)         = delete;
    Random& operator=(const Random&)    = delete;

public:
    u32 seed(void) noexcept;
    void seed(u32) noexcept;
    bool chance(f32) noexcept;
    u32 randSeed(void) noexcept;
    void rollSeed(void) noexcept;
    std::vector<u8> dice(u8, u8) noexcept;
    
public:
    template<typename T> T choice(std::span<T>)        		  noexcept;
    template<IsBasicType T  = i32> T rand(const T&, const T&) noexcept;

public:
    Color mixColor(f32 = 1.0f, f32 = 0.5f) 			noexcept;
    std::string choice(std::span<std::string_view>) noexcept;
    
public:
    static Random& getInstance(void) noexcept;
};

inline Random& random = Random::getInstance();

template<> Vec2d  Random::rand<Vec2d>(const Vec2d&, const Vec2d&)    noexcept;
template<> Fact2d Random::rand<Fact2d>(const Fact2d&, const Fact2d&) noexcept;
template<> Grid2d Random::rand<Grid2d>(const Grid2d&, const Grid2d&) noexcept;
template<> Dim2d  Random::rand<Dim2d>(const Dim2d&, const Dim2d&)    noexcept;
template<> Color  Random::rand<Color>(const Color&, const Color&)    noexcept;
} //namespace rmk

#include <remake2d/template/random.tpp>

#endif