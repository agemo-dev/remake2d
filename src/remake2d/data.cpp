#include <remake2d/data.hpp>

#include <map>
#include <string>
#include <vector>
#include <utility>
#include <filesystem>
#include <stdio.h>
#include <fstream>

namespace rmk {

nlohmann::json Data::_toJson(void) const {
    return std::visit([](const auto& v) -> nlohmann::json {
        using T = std::decay_t<decltype(v)>;

        if constexpr (std::is_same_v<T, std::map<std::string, Data>>) {
            nlohmann::json obj;
            for (const auto& [k, d] : v)
                obj[k] = d._toJson();
            return obj;
        }
        else if constexpr (std::is_same_v<T, std::vector<Data>>) {
            nlohmann::json arr = nlohmann::json::array();
            for (const auto& d : v) arr.push_back(d._toJson());
            return arr;
        }
        else if constexpr (std::is_same_v<T, Vec2d>) {
            return nlohmann::json{{"x", v.x}, {"y", v.y}};
        }
        else if constexpr (std::is_same_v<T, Fact2d>) {
            return nlohmann::json{{"x", v.x}, {"y", v.y}};
        }
        else if constexpr (std::is_same_v<T, Grid2d>) {
            return nlohmann::json{{"x", v.x}, {"y", v.y}};
        }
        else if constexpr (std::is_same_v<T, Dim2d>) {
            return nlohmann::json{{"w", v.w}, {"h", v.h}};
        }
        else if constexpr (std::is_same_v<T, Color>) {
            return nlohmann::json{{"r", v.r}, {"g", v.g}, {"b", v.b}, {"a", v.a}};
        }
        else if constexpr (std::is_same_v<T, Area>) {
            return nlohmann::json{{"x", v.x}, {"y", v.y}, {"w", v.w}, {"h", v.h}};
        }
        else if constexpr (std::is_same_v<T, Nil>) {
            return nlohmann::json{};
        }
        else {
            return v;
        }
    }, value);
}

Data Data::_fromJson(const nlohmann::json& j) {
    if (j.is_object()) {
        std::map<std::string, Data> m;
        if (j.contains("x") && j.contains("y") && j.size() == 2)
            return Data(Vec2d{j["x"].get<f32>(), j["y"].get<f32>()});
        if (j.contains("w") && j.contains("h") && j.size() == 2)
            return Data(Dim2d{j["w"].get<f32>(), j["h"].get<f32>()});
        if (j.contains("r") && j.contains("g") && j.contains("b") && j.contains("a") && j.size() == 4)
            return Data(Color{j["r"].get<byte>(), j["g"].get<byte>(), j["b"].get<byte>(), j["a"].get<byte>()});
        if (j.contains("x") && j.contains("y") && j.contains("w") && j.contains("h") && j.size() == 4)
            return Data(Area{j["x"].get<i32>(), j["y"].get<i32>(), j["w"].get<i32>(), j["h"].get<i32>()});
        for (auto& [k, v] : j.items()) m[k] = _fromJson(v);
        return Data(std::move(m));
    }
    if (j.is_array()) {
        std::vector<Data> vec;
        for (const auto& v : j)
            vec.push_back(_fromJson(v));
        return Data(std::move(vec));
    }
    if (j.is_string())  return Data(j.get<std::string>());
    if (j.is_boolean()) return Data(j.get<bool>());
    if (j.is_number_float())   return Data(j.get<fmax>());
    if (j.is_number_integer()) return Data(j.get<imax>());

    return Data(static_cast<imax>(0));
}


DataFile::DataFile(std::string_view name) : m_name(name) {
    if (!data.isInitialized()) data._init();
    std::filesystem::path p = data.root();
    p /= std::string(name) + ".json";
    m_path = p.string();
}

bool DataFile::exist(void) const noexcept {
    return std::filesystem::exists(m_path);
}

void DataFile::remove(void) noexcept {
    std::error_code ec;
    std::filesystem::remove(m_path, ec);
}

std::string DataFile::path(void)   const noexcept {
    return m_path;
}

std::string DataFile::name(void)   const noexcept {
    return m_name;
}

SaveManager& SaveManager::getInstance(void) noexcept {
    static SaveManager instance;
    return instance;
}

void SaveManager::root(std::string_view path) noexcept {
    m_root = std::string(path);
    if (m_initialized) std::filesystem::create_directories(m_root);
}

std::string SaveManager::root(void) const noexcept {
    return m_root;
}

bool SaveManager::isInitialized(void) const noexcept { 
    return m_initialized;
}

void DataFile::save(const Data& dat) {
    nlohmann::json j = dat._toJson();
    std::ofstream file(m_path);
    file << j.dump(4);
}

void DataFile::load(Data& dat) {
    if (!std::filesystem::exists(m_path)) {
        rmk_dynamicAssert(rmk::DataError, error::data::file_not_found);
    }
    std::ifstream file(m_path);
    nlohmann::json j;
    file >> j;
    dat = Data::_fromJson(j);
}

void DataFile::save(const ISavable& obj) {
    Data d = obj.sdata();
    nlohmann::json j = d._toJson();
    std::ofstream file(m_path);
    file << j.dump(4);
}

void DataFile::load(ISavable& obj) {
    if (!std::filesystem::exists(m_path)) {
        rmk_dynamicAssert(rmk::DataError, error::data::file_not_found);
    }
    std::ifstream file(m_path);
    nlohmann::json j;
    file >> j;
    Data d = Data::_fromJson(j);
    obj.ldata(d);
}

void SaveManager::_init(void) {
    std::filesystem::create_directories(m_root);
    m_initialized = true;
}

} // namespace rmk