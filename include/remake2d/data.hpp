#ifndef REMAKE2D_DATA_
#define REMAKE2D_DATA_

#include <remake2d/error.hpp>
#include <remake2d/color.hpp>
#include <remake2d/vector.hpp>
#include <remake2d/utility.hpp>
#include <remake2d/numeric.hpp>
#include <remake2d/concept.hpp>
#include <remake2d/json/json.hpp>

#include <map>
#include <list>
#include <vector>
#include <string>
#include <variant>
#include <utility>
#include <algorithm>
#include <filesystem>
#include <string_view>


namespace rmk {

namespace fdata {
inline static constexpr const char *droot = "data/remake2d/json";
} //namespace rmk


struct Data {
    using Value = std::variant<
        Nil,
        byte,
        rune,
        imax,
        fmax,
        bool,
        Vec2d,
        Fact2d,
        Grid2d,
        Dim2d,
        Color,
        Area,
        std::string,
        std::vector<Data>,
        std::map<std::string, Data>
    >;

    Value value;

    Data(void)                  : value(0) {}
    Data(Nil v)                 : value(0) {}
    Data(byte v)                : value(v) {}
    Data(rune v)                : value(v) {}
    Data(imax v)                : value(v) {}
    Data(fmax v)                : value(v) {}
    Data(bool v)                : value(v) {}
    Data(Vec2d v)               : value(v) {}
    Data(Fact2d v)              : value(v) {}
    Data(Grid2d v)              : value(v) {}
    Data(Dim2d v)               : value(v) {}
    Data(Color v)               : value(v) {}
    Data(Area v)                : value(v) {}
    Data(std::vector<Data> v)   : value(std::move(v)) {}
    Data(std::string_view v)    : value(std::string(v)) {}
    Data(std::map<std::string, Data> v) : value(std::move(v)) {}

    Data(std::span<std::pair<const std::string, Data>> list)
        : value(std::map<std::string, Data>(list.begin(), list.end())) {}

    static Data map(std::span<std::pair<const std::string, Data>> list) {
        return Data(list);
    }
    static Data list(std::span<Data> items) {
        return Data(std::vector<Data>(items.begin(), items.end()));
    }

    const Data& operator[](std::string_view key) const {
        const auto& m = std::get<std::map<std::string, Data>>(value);
        auto it = m.find(std::string(key));
        if (it == m.end()) throw DataError(error::data::invalid_field);
        return it->second;
    }
    
    const Data& operator[](const char* key) const {
        return (*this)[std::string_view(key)];
    }
    
    const Data& operator[](usize idx) const {
        const auto& v = std::get<std::vector<Data>>(value);
        if (idx >= v.size()) throw DataError(error::data::invalid_field);
        return v[idx];
    }

    template<typename T>
    T get(std::string_view key) const {
        const Data& d = (*this)[key];
        if (!std::holds_alternative<T>(d.value)) throw DataError(error::data::invalid_type);
        return std::get<T>(d.value);
    }

    template<typename T>
    operator T() const {
        if (!std::holds_alternative<T>(value))
            throw DataError(error::data::invalid_type);
        return std::get<T>(value);
    }

    nlohmann::json _toJson(void) const;
    static Data _fromJson(const nlohmann::json&);
};


class ISavable {
public:
    virtual Data        sdata(void) const  = 0;
    virtual void        ldata(const Data&) = 0;

public:
    virtual ~ISavable(void) = default;
};


class SaveManager;

class DataFile {
private:
    std::string    m_name;
    std::string    m_path;

public:
    explicit DataFile(std::string_view);

    DataFile(const DataFile&)            = default;
    DataFile(DataFile&&)                 = default;
    DataFile& operator=(DataFile&&)      = default;
    DataFile& operator=(const DataFile&) = default;

    void load(Data&);
    void save(const Data&);
    void load(ISavable&);
    void save(const ISavable&);

    void remove(void) 			 noexcept;
    bool exist(void) 	   const noexcept;
    std::string path(void) const noexcept;
    std::string name(void) const noexcept;

private:
    template<IsSavable T> friend rmk::DataFile& operator>>(rmk::DataFile&, T&);
    template<IsSavable T> friend rmk::DataFile& operator<<(rmk::DataFile&, const T&);
};


class SaveManager {
private:
    bool               m_initialized{false};
    std::string        m_root{fdata::droot};

private:
    SaveManager(void) = default;
    SaveManager(const SaveManager&)            = delete;
    SaveManager& operator=(const SaveManager&) = delete;

public:
    std::string root(void) 			const noexcept;
    bool isInitialized(void) 		const noexcept;
    void root(std::string_view)      	  noexcept;
    static SaveManager& getInstance(void) noexcept;

private:
    void _init(void);
    
private:
    friend class DataFile;
};

inline SaveManager& data = SaveManager::getInstance();

} // namespace rmk
#endif