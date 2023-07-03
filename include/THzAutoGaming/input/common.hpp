#ifndef THZ_AUTO_GAMING_INPUT_COMMON_HPP
#define THZ_AUTO_GAMING_INPUT_COMMON_HPP

#include <concepts>
#include <cstdint>

namespace Terrahertz::Input {

/// @brief Enumeration of all common mouse buttons.
enum class MouseButton : std::uint8_t
{
    Left     = 0x1,
    Right    = 0x2,
    Middle   = 0x4,
    XButton1 = 0x5,
    XButton2 = 0x6
};

/// @brief Enumeration of common keyboard locks.
enum class KeyboardLock : std::int32_t
{
    Scroll = 0x91,
    Num    = 0x90,
    Caps   = 0x14
};

// clang-format off

/// @brief Concept for the system interface used to 
template <typename TInterfaceType>
concept SystemInterface = requires(TInterfaceType systemInterface) {
    // copy constructible
    TInterfaceType(systemInterface);
};

// clang-format on

} // namespace Terrahertz::Input

#endif // !THZ_AUTO_GAMING_INPUT_COMMON_HPP
