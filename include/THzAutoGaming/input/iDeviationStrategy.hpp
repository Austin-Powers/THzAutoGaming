#ifndef THZ_AUTO_GAMING_INPUT_IDEVIATIONSTRATEGY_HPP
#define THZ_AUTO_GAMING_INPUT_IDEVIATIONSTRATEGY_HPP

#include "THzCommon/math/point.hpp"
#include "THzCommon/math/rectangle.hpp"

#include <chrono>

namespace Terrahertz::Input {

/// @brief Interface for calculating the deviation of the inputs done by an emulator.
class IDeviationStrategy
{
public:
    /// @brief Default the destructor to make it virtual.
    [[nodiscard]] virtual ~IDeviationStrategy() noexcept {};

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

    /// @brief Calculates the speed of the cursor on the x coordinate [pxl/s].
    ///
    /// @return The speed of the cursor on the x coordinate [pxl/s].
    [[nodiscard]] virtual std::uint32_t calculateSpeedX() noexcept = 0;

    /// @brief Calculates the speed of the cursor on the y coordinate [pxl/s].
    ///
    /// @return The speed of the cursor on the y coordinate [pxl/s].
    [[nodiscard]] virtual std::uint32_t calculateSpeedY() noexcept = 0;
};

} // namespace Terrahertz::Input

#endif // !THZ_AUTO_GAMING_INPUT_IDEVIATIONSTRATEGY_HPP
