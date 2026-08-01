#ifndef REMAKE2D_CONFIG_
#define REMAKE2D_CONFIG_

namespace rmk {
namespace config {

namespace system {
void initSDL(void);
} // namespace system

namespace sound {
void initQueue(void) noexcept;
} // namespace sound

} // namespace config
} // namespace rmk

#endif