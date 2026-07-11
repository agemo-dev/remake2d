#ifndef REMAKE2D_STREAM_TPP_
#define REMAKE2D_STREAM_TPP_

#include <fstream>

namespace rmk {

template<IsBasicType T> Text& operator<<(Text& t, const T& value) noexcept {
    std::ostringstream oss;
    oss << value;
    return t << oss.str();
}

template<IsSavable T> DataFile& operator<<(DataFile& dfile, const T& obj) {
    
    Data d = obj.sdata();
    nlohmann::json j = d._toJson();
    std::ofstream file(dfile.m_path);
	
    file << j.dump(4);
    return dfile;
}

template<IsSavable T> DataFile& operator>>(DataFile& dfile, T& obj) {

    if (!std::filesystem::exists(dfile.m_path)) {
		rmk_dynamicAssert(rmk::DataError, error::data::file_not_found);
	}
	
    nlohmann::json j;
    std::ifstream file(dfile.m_path);
	
    file >> j;
    Data d = Data::_fromJson(j);
    obj.ldata(d);
    return dfile;
}

}//namespace rmk

#endif