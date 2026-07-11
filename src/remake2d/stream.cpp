#include <remake2d/stream.hpp>

#include <ostream>
#include <istream>
#include <string>
#include <algorithm>
#include <fstream>

namespace rmk {
    
std::ostream& operator<<(std::ostream& os, const Vec2d& vec) noexcept {
    os << "[ x : " << vec.x << "  |  y : " << vec.y << " ]";
    return os;
}
    
std::ostream& operator<<(std::ostream& os, const Fact2d& fac) noexcept {
    os << "[ x : " << fac.x << "  |  y : " << fac.y << " ]";
    return os;
}

std::ostream& operator<<(std::ostream& os, const Dim2d& dim) noexcept {
    os << "[ w : " << dim.w << "  |  h : " << dim.h << " ]";
    return os;
}

std::ostream& operator<<(std::ostream& os, const Color& c) noexcept {
    os << "[ r : " << (int)c.r << "  |  g : " << (int)c.g  << "  |  b : " << (int)c.b  << "  |  a : " << (int)c.a << " ]";
    return os;
}

    
std::ostream& operator<<(std::ostream& os, const Area& a) noexcept {
    os << "[ x : " << a.x << "  |  y : " << a.y  << "  |  w : " << a.w  << "  |  h : " << a.h << " ]";
    return os;
}

Text& operator<<(Text& tx1, Text& tx2) noexcept {
    if(&tx1 != &tx2) tx1.append(tx2.text());
    return tx1;
}

Text& operator<<(Text& tx, const std::string_view s) noexcept {
    tx.append(s);
    return tx;
}
	
Text& operator<<(Text& tx, const fmt t) noexcept {
    tx.append(t);
    return tx;
}

std::istream& operator>>(std::istream& is, Vec2d& vec) {
    std::string str;
    std::getline(is, str);
    auto pos = str.find(',') + 1;
    vec.x = std::stof(str.substr(0));
    vec.y = std::stof(str.substr(pos));
    return is;
}

std::istream& operator>>(std::istream& is, Fact2d& fac) {
    std::string str;
    std::getline(is, str);
    auto pos = str.find(',') + 1;
    fac.x = (u32)std::stoi(str.substr(0));
    fac.y = (u32)std::stoi(str.substr(pos));
    return is;
}

std::istream& operator>>(std::istream& is, Dim2d& dim) {
    std::string str;
    std::getline(is, str);
    auto pos = str.find(',') + 1;
    dim.w = std::stof(str.substr(0));
    dim.h = std::stof(str.substr(pos));
    return is;
}

std::istream& operator>>(std::istream& is, Color& c) {
    std::string str;
    std::getline(is, str);
    auto pos1 = str.find(',') + 1;
    auto pos2 = str.find(',', pos1) + 1;
    auto pos3 = str.find(',', pos2) + 1;
    c.r = std::stof(str);
    c.g = std::stof(str.substr(pos1));
    c.b = std::stof(str.substr(pos2));
    c.a = std::stof(str.substr(pos3));
    return is;
}

std::istream& operator>>(std::istream& is, Area& a) {
    std::string str;
    std::getline(is, str);
    auto pos1 = str.find(',') + 1;
    auto pos2 = str.find(',', pos1) + 1;
    auto pos3 = str.find(',', pos2) + 1;
    a.x = std::stoi(str);
    a.y = std::stoi(str.substr(pos1));
    a.w = std::stoi(str.substr(pos2));
    a.h = std::stoi(str.substr(pos3));
    return is;
}

std::istream& operator>>(std::istream& is, Text& text) noexcept {
    std::string str;
    std::getline(is, str);
    text.write(str);
    return is;
}

}