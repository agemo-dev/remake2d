#ifndef REMAKE2D_STREAM_
#define REMAKE2D_STREAM_

#include <remake2d/data.hpp>
#include <remake2d/texture.hpp>
#include <remake2d/concept.hpp>

#include <ostream>
#include <istream>
#include <sstream>

namespace rmk {
    
std::ostream& operator<<(std::ostream&, const Vec2d&)  noexcept;
std::ostream& operator<<(std::ostream&, const Fact2d&) noexcept;
std::ostream& operator<<(std::ostream&, const Dim2d&)  noexcept;
std::ostream& operator<<(std::ostream&, const Color&)  noexcept;
std::ostream& operator<<(std::ostream&, const Area&)   noexcept;


Text& operator<<(Text&, Text&)            noexcept;
Text& operator<<(Text&, std::string_view) noexcept;
Text& operator<<(Text&, fmt)		      noexcept;
template<IsBasicType T> Text& operator<<(Text& t, const T& value) noexcept;

std::istream& operator>>(std::istream&, Vec2d&);
std::istream& operator>>(std::istream&, Fact2d&);
std::istream& operator>>(std::istream&, Dim2d&);
std::istream& operator>>(std::istream&, Color&);
std::istream& operator>>(std::istream&, Area&);
	
std::istream& operator>>(std::istream&, Text&) noexcept;


template<IsSavable T> DataFile& operator>>(DataFile&, T&);
template<IsSavable T> DataFile& operator<<(DataFile&, const T&);


} //namespace rmk

#include <remake2d/template/stream.tpp>

#endif