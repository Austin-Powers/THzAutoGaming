#ifndef THZ_AUTOGAMING_INPUT_IDEVIATIONSTRATEGY_HPP
#define THZ_AUTOGAMING_INPUT_IDEVIATIONSTRATEGY_HPP

#include "THzCommon/math/point.hpp"
#include "THzCommon/math/rectangle.hpp"

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
    [[nodiscard]] virtual std::chrono::milliseconds calculateKeyDownTime() noexcept = 0;

    /// @brief Calculates the duration for which a key has to be in the up position before the next action on the
    /// keyboard.
    ///
    /// @return The duration for which a key has to be in the up position.
    [[nodiscard]] virtual std::chrono::milliseconds calculateKeyUpTime() noexcept = 0;

    /// @brief Calculates the duration for which a button has to be in the down position before the next action on the
    /// mouse.
    ///
    /// @return The duration for which a button has to be in the down position.
    [[nodiscard]] virtual std::chrono::milliseconds calculateButtonDownTime() noexcept = 0;

    /// @brief Calculates the duration for which a button has to be in the up position before the next action on the
    /// mouse.
    ///
    /// @return The duration for which a button has to be in the up position.
    [[nodiscard]] virtual std::chrono::milliseconds calculateButtonUpTime() noexcept = 0;

    /// @brief Calculates a target location inside the given area.
    ///
    /// @param area The area the target should be in.
    /// @return The point representing the target.
    [[nodiscard]] virtual Point calculateTargetIn(Rectangle const &area) noexcept = 0;

    /// @brief Calculates the vertical cursor speed [pxl/s].
    ///
    /// @return The vertical cursor speed [pxl/s].
    [[nodiscard]] virtual std::uint32_t calculateSpeed() noexcept = 0;

    /// @brief Calculates the factor to get from vertical to horizontal cursor speed.
    ///
    /// @return The factor to get from vertical to horizontal cursor speed.
    [[nodiscard]] virtual double calculateHorizontalSpeedFactor() noexcept = 0;

    /// @brief Calculates the next amount of steps done by the wheel.
    ///
    /// @param remainingSteps The remaining steps to be done.
    /// @return The next amount of steps done by the wheel.
    [[nodiscard]] virtual std::int16_t calculateWheelSteps(std::int16_t const remainingSteps) noexcept = 0;

    /// @brief Calculates the speed the wheel is turned at [steps/s].
    ///
    /// @return The speed the wheel is turned at [steps/s].
    [[nodiscard]] virtual std::uint16_t calculateWheelSpeed() noexcept = 0;

    /// @brief Calculates the next reset interval for the mouse wheel.
    ///
    /// @return The next interval between to mousewheel turns.
    [[nodiscard]] virtual std::chrono::milliseconds calculateWheelResetTime() noexcept = 0;
};

} // namespace Terrahertz::Input

#endif // !THZ_AUTOGAMING_INPUT_IDEVIATIONSTRATEGY_HPP
