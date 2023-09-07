namespace Terrahertz::Input {

#include <chrono>

/// @brief Interface for calculating the deviation of the inputs done by an emulator.
class IDeviationStrategy
{
    virtual std::chrono::milliseconds calculateKeyDownTime() noexcept    = 0;
    virtual std::chrono::milliseconds calculateKeyUpTime() noexcept      = 0;
    virtual std::chrono::milliseconds calculateButtonDownTime() noexcept = 0;
    virtual std::chrono::milliseconds calculateButtonUpTime() noexcept   = 0;
};

} // namespace Terrahertz::Input
