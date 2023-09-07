#ifndef THZ_AUTO_GAMING_INPUT_PARAMETERES_HPP
#define THZ_AUTO_GAMING_INPUT_PARAMETERES_HPP

#include <chrono>
#include <optional>
#include <random>

namespace Terrahertz::Input {

/// @brief The parameter for the input emulator.
class Parameters
{
public:
    /// @brief Tries to create a set of parameters from the given values.
    ///
    /// @param pKeyDownTime The duration a key is pressed.
    /// @param pKeyUpTime The duration between releasing a key and pressing the next.
    /// @param pButtonDownTime The duration a mouse button is pressed.
    /// @param pButtonUpTime The duration between releasing a mouse button and pressing the next.
    /// @param pCursorAccuracy The standard deviation of clicks from the center of the target, mean has to be 0.0.
    /// @param pCursorSpeedX The horizontal cursor speed [pxl/s].
    /// @param pCursorSpeedY The vertical cursor speed [pxl/s].
    /// @return The parameter set, if given values are within the correct boundaries.
    static std::optional<Parameters> create(std::normal_distribution<> const pKeyDownTime,
                                            std::normal_distribution<> const pKeyUpTime,
                                            std::normal_distribution<> const pButtonDownTime,
                                            std::normal_distribution<> const pButtonUpTime,
                                            std::normal_distribution<> const pCursorAccuracy,
                                            std::normal_distribution<> const pCursorSpeedX,
                                            std::normal_distribution<> const pCursorSpeedY) noexcept;

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
    inline std::normal_distribution<> keyDownTime() const noexcept { return _keyDownTime; }

    /// @brief Returns the duration between releasing a key and pressing the next.
    ///
    /// @return The duration between releasing a key and pressing the next.
    inline std::normal_distribution<> keyUpTime() const noexcept { return _keyUpTime; }

    /// @brief Returns the duration a mouse button is pressed.
    ///
    /// @return The duration a mouse button is pressed.
    inline std::normal_distribution<> buttonDownTime() const noexcept { return _buttonDownTime; }

    /// @brief Returns the duration between releasing a mouse button and pressing the next.
    ///
    /// @return The duration between releasing a mouse button and pressing the next.
    inline std::normal_distribution<> buttonUpTime() const noexcept { return _buttonUpTime; }

    /// @brief Returns the standard deviation of clicks from the center of the target.
    ///
    /// @return The standard deviation of clicks from the center of the target.
    inline std::normal_distribution<> cursorAccuracy() const noexcept { return _cursorAccuracy; }

    /// @brief Returns the horizontal cursor speed [pxl/s].
    ///
    /// @return The horizontal cursor speed [pxl/s].
    inline std::normal_distribution<> cursorSpeedX() const noexcept { return _cursorSpeedX; }

    /// @brief Returns the vertical cursor speed [pxl/s].
    ///
    /// @return The vertical cursor speed [pxl/s].
    inline std::normal_distribution<> cursorSpeedY() const noexcept { return _cursorSpeedY; }

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
    Parameters(std::normal_distribution<> const pKeyDownTime,
               std::normal_distribution<> const pKeyUpTime,
               std::normal_distribution<> const pButtonDownTime,
               std::normal_distribution<> const pButtonUpTime,
               std::normal_distribution<> const pCursorAccuracy,
               std::normal_distribution<> const pCursorSpeedX,
               std::normal_distribution<> const pCursorSpeedY) noexcept;

    /// @brief The duration a key is pressed.
    std::normal_distribution<> _keyDownTime;

    /// @brief The duration between releasing a key and pressing the next.
    std::normal_distribution<> _keyUpTime;

    /// @brief The duration a mouse button is pressed.
    std::normal_distribution<> _buttonDownTime;

    /// @brief The duration between releasing a mouse button and pressing the next.
    std::normal_distribution<> _buttonUpTime;

    /// @brief The standard deviation of clicks from the center of the target.
    std::normal_distribution<> _cursorAccuracy;

    /// @brief The horizontal cursor speed [pxl/s].
    std::normal_distribution<> _cursorSpeedX;

    /// @brief The vertical cursor speed [pxl/s].
    std::normal_distribution<> _cursorSpeedY;
};

} // namespace Terrahertz::Input

#endif // !THZ_AUTO_GAMING_INPUT_PARAMETERES_HPP
