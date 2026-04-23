#ifndef THZ_AUTOGAMING_INPUT_COMMON_HPP
#define THZ_AUTOGAMING_INPUT_COMMON_HPP

#include <concepts>
#include <cstdint>

namespace Terrahertz::Input {

/// @brief Enumeration of all common mouse buttons.
enum class MouseButton : std::uint8_t
{
    None     = 0x00U,
    Left     = 0x01U,
    Right    = 0x02U,
    Middle   = 0x04U,
    XButton1 = 0x05U,
    XButton2 = 0x06U

};

/// @brief Array of all types of mouse buttons.
static const MouseButton MouseButtons[] = {
    MouseButton::Left, MouseButton::Right, MouseButton::Middle, MouseButton::XButton1, MouseButton::XButton2};

/// @brief Enumeration of common keyboard locks.
enum class KeyboardLock : std::uint8_t
{
    None   = 0x00U,
    Scroll = 0x91U,
    Num    = 0x90U,
    Caps   = 0x14U
};

/// @brief Array of all types of keyboard locks.
static const KeyboardLock KeyboardLocks[] = {KeyboardLock::Scroll, KeyboardLock::Num, KeyboardLock::Caps};

/// @brief Enumeration of keys on the keyboard.
enum class Key : std::uint8_t
{
    None            = 0x00U,
    Backspace       = 0x08U,
    Tab             = 0x09U,
    Return          = 0x0DU,
    ShiftKey        = 0x10U,
    ControlKey      = 0x11U,
    AltKey          = 0x12U,
    Pause           = 0x13U,
    CapsLock        = 0x14U,
    Escape          = 0x1BU,
    Space           = 0x20U,
    PageUp          = 0x21U,
    PageDown        = 0x22U,
    End             = 0x23U,
    Pos1            = 0x24U,
    LeftArrow       = 0x25U,
    UpArrow         = 0x26U,
    RightArrow      = 0x27U,
    DownArrow       = 0x28U,
    Print           = 0x2AU,
    PrintScreen     = 0x2CU,
    Insert          = 0x2DU,
    Delete          = 0x2EU,
    Number0         = 0x30U,
    Number1         = 0x31U,
    Number2         = 0x32U,
    Number3         = 0x33U,
    Number4         = 0x34U,
    Number5         = 0x35U,
    Number6         = 0x36U,
    Number7         = 0x37U,
    Number8         = 0x38U,
    Number9         = 0x39U,
    A               = 0x41U,
    B               = 0x42U,
    C               = 0x43U,
    D               = 0x44U,
    E               = 0x45U,
    F               = 0x46U,
    G               = 0x47U,
    H               = 0x48U,
    I               = 0x49U,
    J               = 0x4AU,
    K               = 0x4BU,
    L               = 0x4CU,
    M               = 0x4DU,
    N               = 0x4EU,
    O               = 0x4FU,
    P               = 0x50U,
    Q               = 0x51U,
    R               = 0x52U,
    S               = 0x53U,
    T               = 0x54U,
    U               = 0x55U,
    V               = 0x56U,
    W               = 0x57U,
    X               = 0x58U,
    Y               = 0x59U,
    Z               = 0x5AU,
    LWin            = 0x5BU,
    RWin            = 0x5CU,
    NumPad0         = 0x60U,
    NumPad1         = 0x61U,
    NumPad2         = 0x62U,
    NumPad3         = 0x63U,
    NumPad4         = 0x64U,
    NumPad5         = 0x65U,
    NumPad6         = 0x66U,
    NumPad7         = 0x67U,
    NumPad8         = 0x68U,
    NumPad9         = 0x69U,
    Multiply        = 0x6AU,
    Add             = 0x6BU,
    NumPadSeparator = 0x6CU,
    Subtract        = 0x6DU,
    Decimal         = 0x6EU,
    Divide          = 0x6FU,
    F1              = 0x70U,
    F2              = 0x71U,
    F3              = 0x72U,
    F4              = 0x73U,
    F5              = 0x74U,
    F6              = 0x75U,
    F7              = 0x76U,
    F8              = 0x77U,
    F9              = 0x78U,
    F10             = 0x79U,
    F11             = 0x7AU,
    F12             = 0x7BU,
    F13             = 0x7CU,
    F14             = 0x7DU,
    F15             = 0x7EU,
    F16             = 0x7FU,
    F17             = 0x80U,
    F18             = 0x81U,
    F19             = 0x82U,
    F20             = 0x83U,
    F21             = 0x84U,
    F22             = 0x85U,
    F23             = 0x86U,
    F24             = 0x87U,
    NumLock         = 0x90U,
    ScrollLock      = 0x91U,
    LShiftKey       = 0xA0U,
    RShiftKey       = 0xA1U,
    LControlKey     = 0xA2U,
    RControlKey     = 0xA3U,
    LAlt            = 0xA4U,
    RAlt            = 0xA5U
};

/// @brief Array of all types of keys.
static const Key Keys[] = {Key::Backspace,
                           Key::Tab,
                           Key::Return,
                           Key::ShiftKey,
                           Key::ControlKey,
                           Key::AltKey,
                           Key::Pause,
                           Key::CapsLock,
                           Key::Escape,
                           Key::Space,
                           Key::PageUp,
                           Key::PageDown,
                           Key::End,
                           Key::Pos1,
                           Key::LeftArrow,
                           Key::UpArrow,
                           Key::RightArrow,
                           Key::DownArrow,
                           Key::Print,
                           Key::PrintScreen,
                           Key::Insert,
                           Key::Delete,
                           Key::Number0,
                           Key::Number1,
                           Key::Number2,
                           Key::Number3,
                           Key::Number4,
                           Key::Number5,
                           Key::Number6,
                           Key::Number7,
                           Key::Number8,
                           Key::Number9,
                           Key::A,
                           Key::B,
                           Key::C,
                           Key::D,
                           Key::E,
                           Key::F,
                           Key::G,
                           Key::H,
                           Key::I,
                           Key::J,
                           Key::K,
                           Key::L,
                           Key::M,
                           Key::N,
                           Key::O,
                           Key::P,
                           Key::Q,
                           Key::R,
                           Key::S,
                           Key::T,
                           Key::U,
                           Key::V,
                           Key::W,
                           Key::X,
                           Key::Y,
                           Key::Z,
                           Key::LWin,
                           Key::RWin,
                           Key::NumPad0,
                           Key::NumPad1,
                           Key::NumPad2,
                           Key::NumPad3,
                           Key::NumPad4,
                           Key::NumPad5,
                           Key::NumPad6,
                           Key::NumPad7,
                           Key::NumPad8,
                           Key::NumPad9,
                           Key::Multiply,
                           Key::Add,
                           Key::NumPadSeparator,
                           Key::Subtract,
                           Key::Decimal,
                           Key::Divide,
                           Key::F1,
                           Key::F2,
                           Key::F3,
                           Key::F4,
                           Key::F5,
                           Key::F6,
                           Key::F7,
                           Key::F8,
                           Key::F9,
                           Key::F10,
                           Key::F11,
                           Key::F12,
                           Key::F13,
                           Key::F14,
                           Key::F15,
                           Key::F16,
                           Key::F17,
                           Key::F18,
                           Key::F19,
                           Key::F20,
                           Key::F21,
                           Key::F22,
                           Key::F23,
                           Key::F24,
                           Key::NumLock,
                           Key::ScrollLock,
                           Key::LShiftKey,
                           Key::RShiftKey,
                           Key::LControlKey,
                           Key::RControlKey,
                           Key::LAlt,
                           Key::RAlt};

// clang-format off

/// @brief Concept for the system interface used to 
template <typename TInterfaceType>
concept SystemInterface = requires(
    TInterfaceType       systemInterface,
    TInterfaceType const cSystemInterface,
    std::uint32_t       &coordinate,
    std::int16_t         steps,
    MouseButton    const mb,
    Key            const k,
    KeyboardLock   const l) {

    // copy constructible
    TInterfaceType(cSystemInterface);

    // copy assignable
    systemInterface = cSystemInterface;

    {systemInterface.isDown(mb)} noexcept -> std::same_as<bool>;
    {systemInterface.isDown(k)} noexcept -> std::same_as<bool>;
    
    {systemInterface.isActive(l)} noexcept -> std::same_as<bool>;

    {systemInterface.getCursorPosition(coordinate, coordinate)} noexcept -> std::same_as<bool>;
    {systemInterface.setCursorPosition(coordinate, coordinate)} noexcept -> std::same_as<bool>;

    {systemInterface.turnMouseWheel(steps)} noexcept ->std::same_as<bool>;

    {systemInterface.down(mb)} noexcept -> std::same_as<bool>;
    {systemInterface.up(mb)} noexcept -> std::same_as<bool>;

    {systemInterface.down(k)} noexcept -> std::same_as<bool>;
    {systemInterface.up(k)} noexcept -> std::same_as<bool>;
};

// clang-format on

} // namespace Terrahertz::Input

#endif // !THZ_AUTOGAMING_INPUT_COMMON_HPP
