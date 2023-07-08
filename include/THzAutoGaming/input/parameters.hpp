#ifndef THZ_AUTO_GAMING_INPUT_PARAMETERES_HPP
#define THZ_AUTO_GAMING_INPUT_PARAMETERES_HPP

#include <chrono>
#include <optional>

namespace Terrahertz::Input {

/// @brief The parameter for the input emulator.
class Parameters
{
public:
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

    /// @brief Shortcut for a timing value.
    using Time = Value<std::chrono::milliseconds>;

    /// @brief Shortcut for a speed value [pxl/s].
    using Speed = Value<double>;

    /// @brief Tries to create a set of parameters from the given values.
    ///
    /// @param pKeyDownTime The duration a key is pressed.
    /// @param pKeyUpTime The duration between releasing a key and pressing the next.
    /// @param pButtonDownTime The duration a mouse button is pressed.
    /// @param pButtonUpTime The duration between releasing a mouse button and pressing the next.
    /// @param pCursorAccuracy The standard deviation of clicks from the center of the target.
    /// @param pCursorSpeedX The horizontal cursor speed [pxl/s].
    /// @param pCursorSpeedY The vertical cursor speed [pxl/s].
    /// @return The parameter set, if given values are within the correct boundaries.
    static std::optional<Parameters> create(Time const   pKeyDownTime,
                                            Time const   pKeyUpTime,
                                            Time const   pButtonDownTime,
                                            Time const   pButtonUpTime,
                                            double const pCursorAccuracy,
                                            Speed const  pCursorSpeedX,
                                            Speed const  pCursorSpeedY) noexcept;

    /// @brief Returns parameters for the input emulator for a more human like input behavior.
    ///
    /// @return Parameters for the input emulator for a more human like input behavior.
    static Parameters Human() noexcept;

    /// @brief Returns parameters for the input emulator for a fast input behavior.
    ///
    /// @return Parameters for the input emulator for a fast input behavior.
    static Parameters Fast() noexcept;

    /// @brief Returns the duration a key is pressed.
    ///
    /// @return The duration a key is pressed.
    inline Time keyDownTime() const noexcept { return _keyDownTime; }

    /// @brief Returns the duration between releasing a key and pressing the next.
    ///
    /// @return The duration between releasing a key and pressing the next.
    inline Time keyUpTime() const noexcept { return _keyUpTime; }

    /// @brief Returns the duration a mouse button is pressed.
    ///
    /// @return The duration a mouse button is pressed.
    inline Time buttonDownTime() const noexcept { return _buttonDownTime; }

    /// @brief Returns the duration between releasing a mouse button and pressing the next.
    ///
    /// @return The duration between releasing a mouse button and pressing the next.
    inline Time buttonUpTime() const noexcept { return _buttonUpTime; }

    /// @brief Returns the standard deviation of clicks from the center of the target.
    ///
    /// @return The standard deviation of clicks from the center of the target.
    /// @remarks 0.0 always in the middle, 1.0 everywhere on the target area is equaly likely.
    inline double cursorAccuracy() const noexcept { return _cursorAccuracy; }

    /// @brief Returns the horizontal cursor speed [pxl/s].
    ///
    /// @return The horizontal cursor speed [pxl/s].
    inline Speed cursorSpeedX() const noexcept { return _cursorSpeedX; }

    /// @brief Returns the vertical cursor speed [pxl/s].
    ///
    /// @return The vertical cursor speed [pxl/s].
    inline Speed cursorSpeedY() const noexcept { return _cursorSpeedY; }

private:
    /// @brief Initializes a new set of parameters using the given values.
    ///
    /// @param pKeyDownTime The duration a key is pressed.
    /// @param pKeyUpTime The duration between releasing a key and pressing the next.
    /// @param pButtonDownTime The duration a mouse button is pressed.
    /// @param pButtonUpTime The duration between releasing a mouse button and pressing the next.
    /// @param pCursorAccuracy The standard deviation of clicks from the center of the target.
    /// @param pCursorSpeedX The horizontal cursor speed [pxl/s].
    /// @param pCursorSpeedY The vertical cursor speed [pxl/s].
    Parameters(Time const   pKeyDownTime,
               Time const   pKeyUpTime,
               Time const   pButtonDownTime,
               Time const   pButtonUpTime,
               double const pCursorAccuracy,
               Speed const  pCursorSpeedX,
               Speed const  pCursorSpeedY) noexcept;

    /// @brief The duration a key is pressed.
    Time _keyDownTime;

    /// @brief The duration between releasing a key and pressing the next.
    Time _keyUpTime;

    /// @brief The duration a mouse button is pressed.
    Time _buttonDownTime;

    /// @brief The duration between releasing a mouse button and pressing the next.
    Time _buttonUpTime;

    /// @brief The standard deviation of clicks from the center of the target.
    ///
    /// @remarks 0.0 always in the middle, 1.0 everywhere on the target area is equaly likely.
    double _cursorAccuracy;

    /// @brief The horizontal cursor speed [pxl/s].
    Speed _cursorSpeedX;

    /// @brief The vertical cursor speed [pxl/s].
    Speed _cursorSpeedY;
};

} // namespace Terrahertz::Input

#endif // !THZ_AUTO_GAMING_INPUT_PARAMETERES_HPP
