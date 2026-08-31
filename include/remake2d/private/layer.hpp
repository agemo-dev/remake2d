#ifndef REMAKE2D_LAYER_
#define REMAKE2D_LAYER_

#include <remake2d/numeric.hpp>

namespace rmk {

enum class layer : i16 {
    min = -256,
    max = 1023,

    ground = min,
    world  = 0,
    sky    = 256,
    ui     = 511,
    log    = 767
};


namespace layer {
i16 ground(u8) noexcept;
i16 world(u8)  noexcept;
i16 sky(u8)    noexcept;
i16 ui(u8)     noexcept;
i16 log(u8)    noexcept;
} // namespace layer

} // namespace rmk

#endif