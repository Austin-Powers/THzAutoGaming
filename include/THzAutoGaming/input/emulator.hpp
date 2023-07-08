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

    // mouse getter

    /// @brief Returns the current position of the cursor on the screen.
    ///
    /// @param x The x coordinate of the cursor.
    /// @param y The y coordinate of the cursor.
    /// @return True if the operation was successfull, false otherwise.
    bool getCursorPosition(std::uint32_t &x, std::uint32_t &y) noexcept { return _interface.getCursorPosition(x, y); }

    // mouse setter
    // keyboard getter
    // keyboard setter
    // convenience methods

private:
    /// @brief The parameters used by the emulator.
    Parameters _parameters;

    /// @brief The interface used by the emulator.
    TSystemInterface _interface;
};

} // namespace Terrahertz::Input

#endif // !THZ_AUTO_GAMING_INPUT_EMULATOR_HPP
