#ifdef _WIN32

#include "THzAutoGaming/input/windowsInterface.hpp"

#include <Windows.h>

namespace Terrahertz::Input {

bool WindowsInterface::isDown(MouseButton const button) const noexcept
{
    return (GetKeyState(static_cast<int>(button) & 0x8000) != 0);
}

bool WindowsInterface::isDown(Key const key) const noexcept
{
    return (GetKeyState(static_cast<int>(key) & 0x8000) != 0);
}

bool WindowsInterface::isActive(KeyboardLock const lock) const noexcept
{
    return ((GetKeyState(static_cast<int>(lock)) & 0x1) != 0);
}

bool WindowsInterface::getCursorPosition(std::uint32_t &x, std::uint32_t &y) const noexcept
{
    POINT pt;
    GetCursorPos(&pt);
    x = pt.x;
    y = pt.y;
    return true;
}

bool WindowsInterface::setCursorPosition(std::uint32_t const x, std::uint32_t const y) const noexcept
{
    return SetCursorPos(x, y) != 0;
}

bool WindowsInterface::turnMouseWheel(std::int16_t const move) const noexcept
{
    INPUT input;
    memset(&input, 0, sizeof(INPUT));
    input.mi.dwFlags |= 0x800;
    input.mi.mouseData = static_cast<decltype(input.mi.mouseData)>(move);
    return SendInput(1, &input, sizeof(INPUT)) == 1;
}

bool WindowsInterface::down(MouseButton const button) const noexcept
{
    INPUT input;
    memset(&input, 0, sizeof(INPUT));
    switch (button)
    {
    case MouseButton::Left:
        input.mi.dwFlags |= 0x02;
        break;
    case MouseButton::Right:
        input.mi.dwFlags |= 0x08;
        break;
    case MouseButton::Middle:
        input.mi.dwFlags |= 0x20;
        break;
    case MouseButton::XButton1:
        input.mi.dwFlags |= 0x80;
        input.mi.mouseData |= 0x1;
        break;
    case MouseButton::XButton2:
        input.mi.dwFlags |= 0x80;
        input.mi.mouseData |= 0x2;
        break;
    }
    return SendInput(1, &input, sizeof(INPUT)) == 1;
}

bool WindowsInterface::up(MouseButton const button) const noexcept
{
    INPUT input;
    memset(&input, 0, sizeof(INPUT));
    switch (button)
    {
    case MouseButton::Left:
        input.mi.dwFlags |= 0x04;
        break;
    case MouseButton::Right:
        input.mi.dwFlags |= 0x10;
        break;
    case MouseButton::Middle:
        input.mi.dwFlags |= 0x40;
        break;
    case MouseButton::XButton1:
        input.mi.dwFlags |= 0x100;
        input.mi.mouseData |= 0x1;
        break;
    case MouseButton::XButton2:
        input.mi.dwFlags |= 0x100;
        input.mi.mouseData |= 0x2;
        break;
    }
    return SendInput(1, &input, sizeof(INPUT)) == 1;
}

bool WindowsInterface::down(Key const key) const noexcept
{
    INPUT input;
    memset(&input, 0, sizeof(INPUT));
    input.type   = 1;
    input.ki.wVk = static_cast<decltype(input.ki.wVk)>(key);
    return SendInput(1, &input, sizeof(INPUT)) == 1;
}

bool WindowsInterface::up(Key const key) const noexcept
{
    INPUT input;
    memset(&input, 0, sizeof(INPUT));
    input.type       = 1;
    input.ki.wVk     = static_cast<decltype(input.ki.wVk)>(key);
    input.ki.dwFlags = 0x2;
    return SendInput(1, &input, sizeof(INPUT)) == 1;
}

} // namespace Terrahertz::Input

#endif // !_WIN32
