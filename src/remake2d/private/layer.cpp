#include <remake2d/private/layer.hpp>

namespace rmk {

namespace layer {

i16 ground(u8 wall) noexcept {
    return wall - 256;
}
i16 world(u8 wall) noexcept {
    return wall;
}
i16 sky(u8 wall) noexcept {
    return wall + 256;
}
i16 ui(u8 wall) noexcept {
    return wall + 511;
}
i16 log(u8 wall) noexcept {
    return wall + 767;
}

} // namespace layer

} // namespace rmk
