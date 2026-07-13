#include <remake2d/random.hpp>
#include <remake2d/utility.hpp>

namespace rmk {

Random::Random(void) {
    rollSeed();
}

Random& Random::getInstance(void) noexcept {
    static Random instance;
    return instance;
}

u32 Random::seed(void) noexcept {
    return m_seed;
}

void Random::seed(u32 s) noexcept {
    m_seed = s;
    return m_gen.seed(s);
}

  u32 Random::randSeed(void) noexcept {
    u32 seed = 0;

    u32 epoch = static_cast<u32>(
		std::chrono::steady_clock::now().time_since_epoch().count()
	);
    std::random_device rd;
    seed ^= u32(rd()) ^ epoch;

    return seed;
}

void Random::rollSeed(void) noexcept {
    m_gen.seed(randSeed());
}

std::vector<u8> Random::dice(u8 n, u8 faces) noexcept {
    std::uniform_int_distribution<i32> dist(1, faces);
    std::vector<u8> rolls;
    rolls.reserve(n);
    for (i32 i = 0; i < n; ++i) {
        rolls.push_back(dist(m_gen));
    }
    return rolls;
}

bool Random::chance(f32 probability) noexcept {
    f32 prob = (f32)std::clamp(probability, 0.0f, 1.0f);
    std::bernoulli_distribution dist(prob);
    return dist(m_gen);
}

template<> Vec2d Random::rand<Vec2d>(const Vec2d& min, const Vec2d& max) noexcept {
    std::uniform_real_distribution<f32> distX(std::min(min.x, max.x), std::max(min.x, max.x));
    std::uniform_real_distribution<f32> distY(std::min(min.y, max.y), std::max(min.y, max.y));
    return { distX(m_gen), distY(m_gen) };
}

template<> Fact2d Random::rand<Fact2d>(const Fact2d& min, const Fact2d& max) noexcept {
    std::uniform_real_distribution<f32> distX(std::min(min.x, max.x), std::max(min.x, max.x));
    std::uniform_real_distribution<f32> distY(std::min(min.y, max.y), std::max(min.y, max.y));
    return { distX(m_gen), distY(m_gen) };
}

template<> Grid2d Random::rand<Grid2d>(const Grid2d& min, const Grid2d& max) noexcept {
    std::uniform_int_distribution<usize> distX(std::min(min.x, max.x), std::max(min.x, max.x));
    std::uniform_int_distribution<usize> distY(std::min(min.y, max.y), std::max(min.y, max.y));
    return { distX(m_gen), distY(m_gen) };
}

template<> Dim2d Random::rand<Dim2d>(const Dim2d& min, const Dim2d& max) noexcept {
    std::uniform_real_distribution<f32> distW(std::min(min.w, max.w), std::max(min.w, max.w));
    std::uniform_real_distribution<f32> distH(std::min(min.h, max.h), std::max(min.h, max.h));
    return { distW(m_gen), distH(m_gen) };
}

template<> Color Random::rand<Color>(const Color& a, const Color& b) noexcept {
    HSL ha = color::RGBToHSL(a);
    HSL hb = color::RGBToHSL(b);

    f32 h_min = std::min(ha.h, hb.h);
    f32 h_max = std::max(ha.h, hb.h);

    if (h_max - h_min > 180.0f) {
        std::swap(h_min, h_max);
        h_min -= 360.0f;
    }

    std::uniform_real_distribution<f32> hue_dist(h_min, h_max);
    f32 h = hue_dist(m_gen);
    if (h < 0.0f) h += 360.0f;

    HSL out;
    out.h = h;
    out.s = (ha.s + hb.s) / 2.0f;
    out.l = (ha.l + hb.l) / 2.0f;

    return color::HSLToRGB(out);
}

Color Random::mixColor(f32 s, f32 l) noexcept {
    std::uniform_real_distribution<f32> distH(0.0f, 360.0f);
    return color::HSLToRGB({ distH(m_gen), s, l});
}

std::string Random::choice(std::span<std::string_view> choices) noexcept {
    std::vector<std::string> vec(choices.begin(), choices.end());
    if(vec.empty()) return nil;
	
    std::uniform_int_distribution<u32> dist(0, vec.empty() ? 0 : vec.size() - 1);
    return vec[dist(m_gen)];
}

}//namespace rmk