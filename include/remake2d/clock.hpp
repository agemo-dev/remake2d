#ifndef REMAKE2D_CLOCK_
#define REMAKE2D_CLOCK_

#include <remake2d/numeric.hpp>
#include <remake2d/error.hpp>

#include <string>
#include <string_view>
#include <map>
#include <array>
#include <vector>
namespace rmk {

class Date {
private:
    u8              m_day{1};
    u8              m_month{1};
    i32             m_year{2000};
    char            m_separator{'-'};
    std::string     m_format{"year-month-day"};

    inline static constexpr const char m_date_separators[] = { '-', '/', ' ', '.', '\0' };

    inline static const std::map<std::string, u8> m_month_map = {
        {"01",1},{"02",2},{"03",3},{"04",4},{"05",5},{"06",6},
        {"07",7},{"08",8},{"09",9},{"10",10},{"11",11},{"12",12},
        {"1",1},{"2",2},{"3",3},{"4",4},{"5",5},{"6",6},
        {"7",7},{"8",8},{"9",9},
        {"january",1},{"february",2},{"march",3},{"april",4},
        {"may",5},{"june",6},{"july",7},{"august",8},
        {"september",9},{"october",10},{"november",11},{"december",12},
        {"jan",1},{"feb",2},{"mar",3},{"apr",4},
        {"jun",6},{"jul",7},{"aug",8},
        {"sep",9},{"oct",10},{"nov",11},{"dec",12}
    };

    inline static const std::map<u8, std::string> m_month_full = {
        {1,"January"},{2,"February"},{3,"March"},{4,"April"},
        {5,"May"},{6,"June"},{7,"July"},{8,"August"},
        {9,"September"},{10,"October"},{11,"November"},{12,"December"}
    };

    inline static const std::map<u8, std::string> m_month_abbr = {
        {1,"Jan"},{2,"Feb"},{3,"Mar"},{4,"Apr"},
        {5,"May"},{6,"Jun"},{7,"Jul"},{8,"Aug"},
        {9,"Sep"},{10,"Oct"},{11,"Nov"},{12,"Dec"}
    };

public:
    Date(u8, u8,i32);
    
    Date(void)                      = default;
    Date(Date&&)                    = default;
    Date(const Date&)               = default;
    Date& operator=(Date&&)         = default;
    Date& operator=(const Date&)    = default;

public:
    u8   day(void)   const noexcept;
    u8   month(void) const noexcept;
    i32  year(void)  const noexcept;
    char slice(void) const noexcept;

    void day(u8 d)   noexcept;
    void month(u8 m) noexcept;
    void year(i32 y) noexcept;

    void format(const std::string_view& fmt);

    void date(const std::string_view& str);
    std::string date(void) const;
    bool dayIs(const Date& other) const noexcept;

private:
    static char _deduce_separator(const std::string_view& str) noexcept;
    static std::vector<std::string> _split(const std::string&, char);
    static std::vector<std::string> _parse_order(const std::string&, char);
    static u8 _parse_month(std::string token);
};


class Clock {
private:
    u8              m_hour{0};
    u8              m_minute{0};
    u8              m_second{0};
    char            m_separator{':'};
    std::string     m_format{"hour:minute:second"};

    inline static constexpr const char m_clock_separators[] = { ':', '-', '*', '\0' };

public:
    Clock(u8, u8, u8);
    
    Clock(void)                     = default;
    Clock(Clock&&)                  = default;
    Clock& operator=(Clock&&)       = default;
    Clock(const Clock& other)       = default;
    Clock& operator=(const Clock&)  = default;

public:
    u8   hour(void)   const noexcept;
    u8   minute(void) const noexcept;
    u8   second(void) const noexcept;
    char slice(void)  const noexcept;

    void hour(const u8&)   noexcept;
    void minute(const u8&) noexcept;
    void second(const u8&) noexcept;
    
    void time(const std::string_view&);
    void format(const std::string_view&);

    std::string time(void) const;
    bool timeIs(const Clock& other) const noexcept;

private:
    static char _deduce_separator(const std::string_view&) noexcept;
    static std::vector<std::string> _split(const std::string&, char);
    static std::vector<std::string> _parse_order(const std::string&, char);
};

} // namespace rmk
#endif