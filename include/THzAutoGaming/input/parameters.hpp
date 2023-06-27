#ifndef THZ_AUTO_GAMING_INPUT_PARAMETERES_HPP
#define THZ_AUTO_GAMING_INPUT_PARAMETERES_HPP

#include <chrono>
#include <cstddef>
#include <cstdint>

namespace Terrahertz::Input {

/// @brief The parameter for the input emulator.
struct Parameters
{
    /// @brief A pair of mean and standard devation from the mean.
    ///
    /// @tparam T The data type of the value.
    template <typename T>
    struct Value
    {
        /// @brief The mean of the value.
        T mean;

        /// @brief The standard deviation from the mean.
        T stddev;
    };

    /// @brief The duration a key is pressed.
    Value<std::chrono::milliseconds> keyDownTime;

    /// @brief The duration between releasing a key and pressing the next.
    Value<std::chrono::milliseconds> keyUpTime;

    /// @brief The duration a mouse button is pressed.
    Value<std::chrono::milliseconds> buttonDownTime;

    /// @brief The duration between releasing a mouse button and pressing the next.
    Value<std::chrono::milliseconds> buttonUpTime;

    /// @brief The standard deviation of clicks from the center of the target.
    ///
    /// @remarks 0.0 always in the middle, 1.0 everywhere on the target area is equaly likely.
    double mouseAccuracy;

    /// @brief The horizontal cursor speed [pxl/s].
    Value<double> cursorSpeedX;

    /// @brief The vertical cursor speed [pxl/s].
    Value<double> cursorSpeedX;
};

/// @brief Parameters for the input emulator for a more human like behavior.
constexpr Parameters Human{55, 24, 108, 130, 77, 14, 76, 21, 0.2821, 492.65, 335.93, 272.98, 211.43};

/// @brief Parameters for the input emulator for a fast input behavior.
constexpr Parameters Fast{50, 0, 50, 0, 50, 0, 50, 0, 0.0, 3840.0, 0.0, 2160.0, 0.0};

} // namespace Terrahertz::Input

#endif // !THZ_AUTO_GAMING_INPUT_PARAMETERES_HPP
