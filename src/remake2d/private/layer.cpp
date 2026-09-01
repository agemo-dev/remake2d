#include <remake2d/private/layer.hpp>

namespace rmk {

namespace layer {

i16 ground(u8 wall) noexcept {
    return std::min(wall, (u8)layer::size - 1) + (i16)layer::ground;
}
i16 world(u8 wall) noexcept {
    return std::min(wall, (u8)layer::size - 1);
}
i16 sky(u8 wall) noexcept {
    return std::min(wall, (u8)layer::size - 1) + (i16)layer::sky;
}
i16 ui(u8 wall) noexcept {
    return std::min(wall, (u8)layer::size - 1) + (i16)layer::ui;
}
i16 log(u8 wall) noexcept {
    return std::min(wall, (u8)layer::size - 1) + (i16)layer::log;
}

} // namespace layer

} // namespace rmk
