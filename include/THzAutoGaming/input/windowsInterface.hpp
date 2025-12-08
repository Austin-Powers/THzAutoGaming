#ifndef THZ_AUTOGAMING_INPUT_WINDOWSINTERFACE_HPP
#define THZ_AUTOGAMING_INPUT_WINDOWSINTERFACE_HPP

// functions used by this class are OS specific
#ifdef _WIN32

#include "THzAutoGaming/input/common.hpp"
#include "THzAutoGaming/input/emulator.hpp"

#include <cstdint>

namespace Terrahertz::Input {

/// @brief Implementation of the SystemInterface concept for windows.
class WindowsInterface
{
public:
    /// @brief The default constructor.
    WindowsInterface() noexcept = default;

    /// @brief Checks if the given mouse button is down.
    ///
    /// @param button The button to check.
    /// @return True if the button is down, false otherwise.
    bool isDown(MouseButton const button) const noexcept;

    /// @brief Checks if the given key is down.
    ///
    /// @param key The key to check.
    /// @return True if the key is down, false otherwise.
    bool isDown(Key const key) const noexcept;

    /// @brief Checks if the given lock is active.
    ///
    /// @param lock The lock to check.
    /// @return True if the lock is active, false otherwise.
    bool isActive(KeyboardLock const lock) const noexcept;

    /// @brief Retrieves the current cursor position.
    ///
    /// @param x The x coordinate of the position.
    /// @param y The y coordinate of the position.
    /// @return True if the operation was successful, false otherwise.
    bool getCursorPosition(std::uint32_t &x, std::uint32_t &y) const noexcept;

    /// @brief Sets the cursor position.
    ///
    /// @param x The x coordinate of the position.
    /// @param y The y coordinate of the position.
    /// @return True if the operation was successful, false otherwise.
    bool setCursorPosition(std::uint32_t const x, std::uint32_t const y) const noexcept;

    /// @brief Turns the mouse wheel.
    ///
    /// @param steps The number of steps to turn, positive is up or forward.
    /// @return True if the operation was successful, false otherwise.
    bool turnMouseWheel(std::int16_t const steps) const noexcept;

    /// @brief Presse the given mouse button.
    ///
    /// @param button The button to press.
    /// @return True if the operation was successful, false otherwise.
    bool down(MouseButton const button) const noexcept;

    /// @brief Releases the given mouse button.
    ///
    /// @param button The button to release.
    /// @return True if the operation was successful, false otherwise.
    bool up(MouseButton const button) const noexcept;

    /// @brief Presse the given key.
    ///
    /// @param button The key to press.
    /// @return True if the operation was successful, false otherwise.
    bool down(Key const key) const noexcept;

    /// @brief Releases the given key.
    ///
    /// @param button The key to release.
    /// @return True if the operation was successful, false otherwise.
    bool up(Key const key) const noexcept;
};

/// @brief Type of an Emulator using the windows interface.
using WindowEmulator = Emulator<WindowsInterface>;

} // namespace Terrahertz::Input

#endif // !_WIN32
#endif // !THZ_AUTOGAMING_INPUT_WINDOWSINTERFACE_HPP
