#include <chrono>

namespace Terrahertz::Input {

/// @brief Interface for calculating the deviation of the inputs done by an emulator.
class IDeviationStrategy
{
public:
    /// @brief Default the destructor to make it virtual.
    virtual ~IDeviationStrategy() noexcept {};

    /// @brief Calculates the duration for which a key has to be in the down position before the next action on the
    /// keyboard.
    ///
    /// @return The duration for which a key has to be in the down position.
    virtual std::chrono::milliseconds calculateKeyDownTime() noexcept = 0;

    /// @brief Calculates the duration for which a key has to be in the up position before the next action on the
    /// keyboard.
    ///
    /// @return The duration for which a key has to be in the up position.
    virtual std::chrono::milliseconds calculateKeyUpTime() noexcept = 0;

    /// @brief Calculates the duration for which a button has to be in the down position before the next action on the
    /// mouse.
    ///
    /// @return The duration for which a button has to be in the down position.
    virtual std::chrono::milliseconds calculateButtonDownTime() noexcept = 0;

    // virtual std::chrono::milliseconds calculateButtonUpTime() noexcept   = 0;
};

} // namespace Terrahertz::Input
