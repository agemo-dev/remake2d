#include <remake2d/clock.hpp>

#include <string>
#include <sstream>
#include <iomanip>
#include <array>
#include <algorithm>
#include <ctype.h>
#include <time.h>
#include <vector>
namespace rmk {

Date::Date(u8 day, u8 month, i32 year) {
    m_day = day;
    m_month = month;
    m_year = year;
}

u8   Date::day(void)   const noexcept { return m_day; }
u8   Date::month(void) const noexcept { return m_month; }
i32  Date::year(void)  const noexcept { return m_year; }
char Date::slice(void) const noexcept { return m_separator; }

void Date::day(u8 d)   noexcept { m_day = d;}
void Date::month(u8 m) noexcept { m_month = m;}
void Date::year(i32 y) noexcept { m_year = y;}

void Date::format(const std::string_view& fmt) {
    char sep = _deduce_separator(fmt);
    if (sep == '\0')  rmk_dynamicAssert(rmk::DateError, error::date::invalid_separator);
    m_separator = sep;
    m_format    = std::string(fmt);
}

void Date::date(const std::string_view& str) {
    char sep = _deduce_separator(str);
    if (sep == '\0')  rmk_dynamicAssert(rmk::DateError, error::date::invalid_separator);
    m_separator = sep;

    auto parts = _split(std::string(str), sep);
    if (parts.size() != 3)  rmk_dynamicAssert(rmk::DateError, error::date::invalid_date);

    auto order = _parse_order(m_format, sep);
    for (int i = 0; i < 3; i++) {
        const std::string& field = order[i];
        const std::string& token = parts[i];

        if (field == "day") {
            m_day = (u8)std::stoi(token);
        } else if (field == "month" || field == "Month" || field == "Mon") {
            u8 m = _parse_month(token);
            if (m == 0)  rmk_dynamicAssert(rmk::DateError, error::date::invalid_month);
            m_month = m;
        } else if (field == "year") {
            m_year = (i32)std::stoi(token);
        }
    }
}

std::string Date::date(void) const {
    auto order = _parse_order(m_format, m_separator);
    std::ostringstream oss;

    for (int i = 0; i < 3; i++) {
        if (i > 0) oss << m_separator;
        const std::string& field = order[i];

        if      (field == "day")   oss << std::setw(2) << std::setfill('0') << (int)m_day;
        else if (field == "month") oss << std::setw(2) << std::setfill('0') << (int)m_month;
        else if (field == "Month") oss << m_month_full.at(m_month);
        else if (field == "Mon")   oss << m_month_abbr.at(m_month);
        else if (field == "year")  oss << m_year;
    }
    return oss.str();
}

bool Date::dayIs(const Date& other) const noexcept {
    return m_year  == other.m_year
        && m_month == other.m_month
        && m_day   == other.m_day;
}

char Date::_deduce_separator(const std::string_view& str) noexcept {
    for (char c : str) for (char sep : m_date_separators) if (c == sep) return c;
    return '\0';
}

std::vector<std::string> Date::_split(const std::string& s, char sep) {
    std::vector<std::string> result;
    std::istringstream iss(s);
    std::string token;
    while (std::getline(iss, token, sep)) result.push_back(token);
    return result;
}

std::vector<std::string>Date::_parse_order(const std::string& fmt, char sep) {
    std::vector<std::string> order;
    std::istringstream iss(fmt);
    std::string token;
    while (std::getline(iss, token, sep)) {
        if (token == "Month" || token == "Mon")
            order.push_back(token);
        else {
            std::transform(token.begin(), token.end(), token.begin(), ::tolower);
            order.push_back(token);
        }
    }
    if (order.size() != 3) return {"year", "month", "day"};
    return order;
}

u8 Date::_parse_month(std::string token) {
    std::transform(token.begin(), token.end(), token.begin(), ::tolower);
    auto it = m_month_map.find(token);
    return (it != m_month_map.end()) ? it->second : 0;
}


Clock::Clock(u8 hour, u8 minute, u8 second) : m_hour(hour), m_minute(minute), m_second(second) {}

u8   Clock::hour(void)   const noexcept { return m_hour; }
u8   Clock::minute(void) const noexcept { return m_minute; }
u8   Clock::second(void) const noexcept { return m_second; }
char Clock::slice(void)  const noexcept { return m_separator; }

void Clock::hour(const u8& h)   noexcept { m_hour = h;}
void Clock::minute(const u8& m) noexcept { m_minute = m;}
void Clock::second(const u8& s) noexcept { m_second = s;}

void Clock::format(const std::string_view& fmt) {
    char sep = _deduce_separator(fmt);
    if (sep == '\0')  rmk_dynamicAssert(rmk::ClockError, error::clock::invalid_separator);
    m_separator = sep;
    m_format    = std::string(fmt);
}

void Clock::time(const std::string_view& str) {
    char sep = _deduce_separator(str);
    if (sep == '\0')  rmk_dynamicAssert(rmk::ClockError, error::clock::invalid_separator);
    m_separator = sep;

    auto parts = _split(std::string(str), sep);
    if (parts.size() != 3)  rmk_dynamicAssert(rmk::ClockError, error::clock::invalid_time);

    auto order = _parse_order(m_format, sep);
    for (int i = 0; i < 3; i++) {
        const std::string& field = order[i];
        u8 val = (u8)std::stoi(parts[i]);

        if      (field == "hour")   m_hour = val;
        else if (field == "minute") m_minute = val;
        else if (field == "second") m_second = val;
    }
}

std::string Clock::time(void) const {
    auto order = _parse_order(m_format, m_separator);
    std::ostringstream oss;

    for (int i = 0; i < 3; i++) {
        if (i > 0) oss << m_separator;
        const std::string& field = order[i];

        if      (field == "hour")   oss << std::setw(2) << std::setfill('0') << (int)m_hour;
        else if (field == "minute") oss << std::setw(2) << std::setfill('0') << (int)m_minute;
        else if (field == "second") oss << std::setw(2) << std::setfill('0') << (int)m_second;
    }
    return oss.str();
}

bool Clock::timeIs(const Clock& other) const noexcept {
    return m_hour   == other.m_hour
        && m_minute == other.m_minute
        && m_second == other.m_second;
}

char Clock::_deduce_separator(const std::string_view& str) noexcept {
    for (char c : str) for (char sep : m_clock_separators) if (c == sep) return c;
    return '\0';
}

std::vector<std::string> Clock::_split(const std::string& s, char sep) {
    std::vector<std::string> result;
    std::istringstream iss(s);
    std::string token;
    while (std::getline(iss, token, sep))
        result.push_back(token);
    return result;
}

std::vector<std::string> Clock::_parse_order(const std::string& fmt, char sep) {
    std::vector<std::string> order;
    std::istringstream iss(fmt);
    std::string token;
    while (std::getline(iss, token, sep)) {
        std::transform(token.begin(), token.end(), token.begin(), ::tolower);
        order.push_back(token);
    }
    if (order.size() != 3) return {"hour", "minute", "second"};
    return order;
}

} //namespace rmk