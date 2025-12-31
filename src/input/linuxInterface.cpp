#ifdef __linux__

#include "THzAutoGaming/input/linuxInterface.hpp"

namespace Terrahertz::Input {

bool LinuxInterface::isDown(MouseButton const button) const noexcept
{
    return false;
}

bool LinuxInterface::isDown(Key const key) const noexcept
{
    return false;
}

bool LinuxInterface::isActive(KeyboardLock const lock) const noexcept
{
    return false;
}

bool LinuxInterface::getCursorPosition(std::uint32_t &x, std::uint32_t &y) const noexcept
{
    return false;
}

bool LinuxInterface::setCursorPosition(std::uint32_t const x, std::uint32_t const y) const noexcept
{
    return false;
}

bool LinuxInterface::turnMouseWheel(std::int16_t const move) const noexcept
{
    return false;
}

bool LinuxInterface::down(MouseButton const button) const noexcept
{
    return false;
}

bool LinuxInterface::up(MouseButton const button) const noexcept
{
    return false;
}

bool LinuxInterface::down(Key const key) const noexcept
{
    return false;
}

bool LinuxInterface::up(Key const key) const noexcept
{
    return false;
}

} // namespace Terrahertz::Input

#endif // __linux__
