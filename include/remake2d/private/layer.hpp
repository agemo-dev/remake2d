#ifndef REMAKE2D_LAYER_
#define REMAKE2D_LAYER_

#include <remake2d/numeric.hpp>

namespace rmk {

enum class layer : i16 {
    min   = -50,
    max   = 249,

    size  = 50, // distance between two layers

    ground = min,
    world  = ground + size,
    sky    = world  + size,
    ui     = sky    + size,
    log    = ui     + size,

    count = log + size * 2 - 1 // total layers count
};


namespace layer {
i16 ground(u8) noexcept;
i16 world(u8)  noexcept;
i16 sky(u8)    noexcept;
i16 ui(u8)     noexcept;
i16 log(u8)    noexcept;
} // namespace layer

} // namespace rmk
q

