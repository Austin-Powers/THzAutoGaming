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

/// @brief Enumeration of keys on the keyboard.
enum class Key : std::uint8_t
{
    Backspace       = 0x08,
    Tab             = 0x09,
    Return          = 0x0D,
    ShiftKey        = 0x10,
    ControlKey      = 0x11,
    AltKey          = 0x12,
    Pause           = 0x13,
    CapsLock        = 0x14,
    Escape          = 0x1B,
    Space           = 0x20,
    PageUp          = 0x21,
    PageDown        = 0x22,
    End             = 0x23,
    Pos1            = 0x24,
    LeftArrow       = 0x25,
    UpArrow         = 0x26,
    RightArrow      = 0x27,
    DownArrow       = 0x28,
    Print           = 0x2A,
    PrintScreen     = 0x2C,
    Insert          = 0x2D,
    Delete          = 0x2E,
    Number0         = 0x30,
    Number1         = 0x31,
    Number2         = 0x32,
    Number3         = 0x33,
    Number4         = 0x34,
    Number5         = 0x35,
    Number6         = 0x36,
    Number7         = 0x37,
    Number8         = 0x38,
    Number9         = 0x39,
    A               = 0x41,
    B               = 0x42,
    C               = 0x43,
    D               = 0x44,
    E               = 0x45,
    F               = 0x46,
    G               = 0x47,
    H               = 0x48,
    I               = 0x49,
    J               = 0x4A,
    K               = 0x4B,
    L               = 0x4C,
    M               = 0x4D,
    N               = 0x4E,
    O               = 0x4F,
    P               = 0x50,
    Q               = 0x51,
    R               = 0x52,
    S               = 0x53,
    T               = 0x54,
    U               = 0x55,
    V               = 0x56,
    W               = 0x57,
    X               = 0x58,
    Y               = 0x59,
    Z               = 0x5A,
    LWin            = 0x5B,
    RWin            = 0x5C,
    NumPad0         = 0x60,
    NumPad1         = 0x61,
    NumPad2         = 0x62,
    NumPad3         = 0x63,
    NumPad4         = 0x64,
    NumPad5         = 0x65,
    NumPad6         = 0x66,
    NumPad7         = 0x67,
    NumPad8         = 0x68,
    NumPad9         = 0x69,
    Multiply        = 0x6A,
    Add             = 0x6B,
    NumPadSeparator = 0x6C,
    Subtract        = 0x6D,
    Decimal         = 0x6E,
    Divide          = 0x6F,
    F1              = 0x70,
    F2              = 0x71,
    F3              = 0x72,
    F4              = 0x73,
    F5              = 0x74,
    F6              = 0x75,
    F7              = 0x76,
    F8              = 0x77,
    F9              = 0x78,
    F10             = 0x79,
    F11             = 0x7A,
    F12             = 0x7B,
    F13             = 0x7C,
    F14             = 0x7D,
    F15             = 0x7E,
    F16             = 0x7F,
    F17             = 0x80,
    F18             = 0x81,
    F19             = 0x82,
    F20             = 0x83,
    F21             = 0x84,
    F22             = 0x85,
    F23             = 0x86,
    F24             = 0x87,
    NumLock         = 0x90,
    ScrollLock      = 0x91,
    LShiftKey       = 0xA0,
    RShiftKey       = 0xA1,
    LControlKey     = 0xA2,
    RControlKey     = 0xA3,
    LAlt            = 0xA4,
    RAlt            = 0xA5,
};

// clang-format off

/// @brief Concept for the system interface used to 
template <typename TInterfaceType>
concept SystemInterface = requires(TInterfaceType systemInterface, TInterfaceType const cSystemInterface, std::uint32_t &coordinate, MouseButton const mb) {
    // copy constructible
    TInterfaceType(cSystemInterface);

    // copy assignable
    systemInterface = cSystemInterface;

    {systemInterface.getCursorPosition(coordinate, coordinate)} noexcept -> std::same_as<bool>;
    
    {systemInterface.setCursorPosition(coordinate, coordinate)} noexcept -> std::same_as<bool>;

    {systemInterface.isDown(mb)} noexcept -> std::same_as<bool>;
};

// clang-format on

} // namespace Terrahertz::Input

#endif // !THZ_AUTO_GAMING_INPUT_COMMON_HPP
