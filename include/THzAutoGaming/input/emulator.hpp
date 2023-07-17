#ifndef THZ_AUTO_GAMING_INPUT_EMULATOR_HPP
#define THZ_AUTO_GAMING_INPUT_EMULATOR_HPP

#include "common.hpp"
#include "parameters.hpp"

namespace Terrahertz::Input {

/// @brief Emulates user input via keyboard and mouse.
template <SystemInterface TSystemInterface>
class Emulator
{
public:
    /// @brief Initializes a new Emulator using the given parameters and interface
    ///
    /// @param parameters
    /// @param sysInterface
    Emulator(Parameters const &pParameters, TSystemInterface pInterface = {}) noexcept
        : _parameters{pParameters}, _interface{pInterface}
    {}

    /// @brief Returns the parameters used by the emulator.
    ///
    /// @return The parameters used by the emulator.
    inline Parameters const &parametes() const noexcept { return _parameters; }

    /// @brief Returns the current position of the cursor on the screen.
    ///
    /// @param x The x coordinate of the cursor.
    /// @param y The y coordinate of the cursor.
    /// @return True if the operation was successfull, false otherwise.
    bool getCursorPosition(std::uint32_t &x, std::uint32_t &y) noexcept { return _interface.getCursorPosition(x, y); }

    /// @brief Returns the state of the given mouse button.
    ///
    /// @param button The button to check.
    /// @return True if the button is down, false otherwise.
    bool isDown(MouseButton const button) noexcept { return _interface.isDown(button); }

    /// @brief Return the state of the given key.
    ///
    /// @param key The key to check.
    /// @return True if the key is down, false otherwise.
    bool isDown(Key const key) noexcept { return _interface.isDown(key); }

    // wait
    // reset
    // clear
    // sync keyboard/mouse

    // move
    // click(at)
    // drag and drop?
    // down/up button

    // enterString
    // key press
    // key down/up

private:
    /// @brief Data structure for a single action performed by the emulator.
    struct Action
    {
        /// @brief The type of action.
        enum class Type : std::uint8_t
        {
            /// @brief No action just wait.
            None,

            /// @brief Release key or button.
            Up,

            /// @brief Push key or button.
            Down,

            /// @brief Moves the cursor to a given position.
            Move
        };

        /// @brief The type of the action.
        Type type;

        /// @brief The key-/button-code of the action.
        std::uint8_t code;

        /// @brief The time point of the next action.
        std::chrono::time_point<std::chrono::system_clock> nextAction;

        /// @brief The x coordinate for move actions.
        std::uint32_t x;

        /// @brief The y coordinate for move actions.
        std::uint32_t y;
    };

    /// @brief The parameters used by the emulator.
    Parameters _parameters;

    /// @brief The interface used by the emulator.
    TSystemInterface _interface;
};

} // namespace Terrahertz::Input

#endif // !THZ_AUTO_GAMING_INPUT_EMULATOR_HPP
