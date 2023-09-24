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
    /// @param pKeyDownTime The duration a key is pressed [ms].
    /// @param pKeyUpTime The duration between releasing a key and pressing the next [ms].
    /// @param pButtonDownTime The duration a mouse button is pressed [ms].
    /// @param pButtonUpTime The duration between releasing a mouse button and pressing the next [ms].
    /// @param pCursorAccuracy The standard deviation of clicks from the center of the target, mean has to be 0.0.
    /// @param pCursorSpeedX The vertical cursor speed [pxl/s].
    /// @param pHorizontalSpeedFactor The factor to get from vertical to horizontal cursor speed.
    /// @param pWheelStepsPerPush The steps the mouse wheel does per of the finger.
    /// @param pWheelSpeed The speed the wheel is turned at [steps/s].
    /// @param pWheelResetTime The time it takes for the finger to reposition for the next push [ms].
    /// @return The parameter set, if given values are within the correct boundaries.
    [[nodiscard]] static std::optional<Parameters> create(std::normal_distribution<> const pKeyDownTime,
                                                          std::normal_distribution<> const pKeyUpTime,
                                                          std::normal_distribution<> const pButtonDownTime,
                                                          std::normal_distribution<> const pButtonUpTime,
                                                          std::normal_distribution<> const pCursorAccuracy,
                                                          std::normal_distribution<> const pCursorSpeed,
                                                          double const                     pHorizontalSpeedFactor,
                                                          std::uint16_t const              pWheelStepsPerPush,
                                                          std::uint16_t const              pWheelSpeed,
                                                          std::normal_distribution<> const pWheelResetTime) noexcept;

    /// @brief Returns parameters for the input emulator for a more human like input behavior.
    ///
    /// @return Parameters for the input emulator for a more human like input behavior.
    static Parameters Human() noexcept;

    /// @brief Returns parameters for the input emulator for a fast input behavior.
    ///
    /// @return Parameters for the input emulator for a fast input behavior.
    static Parameters Fast() noexcept;

    /// @brief Returns the duration a key is pressed [ms].
    ///
    /// @return The duration a key is pressed [ms].
    [[nodiscard]] inline std::normal_distribution<> keyDownTime() const noexcept { return _keyDownTime; }

    /// @brief Returns the duration between releasing a key and pressing the next [ms].
    ///
    /// @return The duration between releasing a key and pressing the next [ms].
    [[nodiscard]] inline std::normal_distribution<> keyUpTime() const noexcept { return _keyUpTime; }

    /// @brief Returns the duration a mouse button is pressed [ms].
    ///
    /// @return The duration a mouse button is pressed [ms].
    [[nodiscard]] inline std::normal_distribution<> buttonDownTime() const noexcept { return _buttonDownTime; }

    /// @brief Returns the duration between releasing a mouse button and pressing the next [ms].
    ///
    /// @return The duration between releasing a mouse button and pressing the next [ms].
    [[nodiscard]] inline std::normal_distribution<> buttonUpTime() const noexcept { return _buttonUpTime; }

    /// @brief Returns the standard deviation of clicks from the center of the target.
    ///
    /// @return The standard deviation of clicks from the center of the target.
    [[nodiscard]] inline std::normal_distribution<> cursorAccuracy() const noexcept { return _cursorAccuracy; }

    /// @brief Returns the vertical cursor speed [pxl/s].
    ///
    /// @return The vertical cursor speed [pxl/s].
    [[nodiscard]] inline std::normal_distribution<> cursorSpeed() const noexcept { return _cursorSpeed; }

    /// @brief Returns the factor to get from vertical to horizontal cursor speed.
    ///
    /// @return The factor to get from vertical to horizontal cursor speed.
    [[nodiscard]] inline double horizontalSpeedFactor() const noexcept { return _horizontalSpeedFactor; }

    /// @brief Returns the steps the mouse wheel does per of the finger.
    ///
    /// @return The steps the mouse wheel does per of the finger.
    [[nodiscard]] inline std::uint16_t wheelStepsPerPush() const noexcept { return _wheelStepsPerPush; }

    /// @brief Returns the speed the wheel is turned at [steps/s].
    ///
    /// @return The speed the wheel is turned at [steps/s].
    [[nodiscard]] inline std::uint16_t wheelSpeed() const noexcept { return _wheelSpeed; }

    /// @brief Return the time it takes for the finger to reposition for the next push [ms].
    ///
    /// @return The time it takes for the finger to reposition for the next push [ms].
    [[nodiscard]] inline std::normal_distribution<> wheelResetTime() const noexcept { return _wheelResetTime; }

private:
    /// @brief Initializes a new set of parameters using the given values.
    ///
    /// @param pKeyDownTime The duration a key is pressed [ms].
    /// @param pKeyUpTime The duration between releasing a key and pressing the next [ms].
    /// @param pButtonDownTime The duration a mouse button is pressed [ms].
    /// @param pButtonUpTime The duration between releasing a mouse button and pressing the next [ms].
    /// @param pCursorAccuracy The standard deviation of clicks from the center of the target.
    /// @param pCursorSpeedX The vertical cursor speed [pxl/s].
    /// @param pHorizontalSpeedFactor The factor to get from vertical to horizontal cursor speed.
    /// @param pWheelStepsPerPush The steps the mouse wheel does per of the finger.
    /// @param pWheelSpeed The speed the wheel is turned at [steps/s].
    /// @param pWheelResetTime The time it takes for the finger to reposition for the next push [ms].
    Parameters(std::normal_distribution<> const pKeyDownTime,
               std::normal_distribution<> const pKeyUpTime,
               std::normal_distribution<> const pButtonDownTime,
               std::normal_distribution<> const pButtonUpTime,
               std::normal_distribution<> const pCursorAccuracy,
               std::normal_distribution<> const pCursorSpeed,
               double const                     pHorizontalSpeedFactor,
               std::uint16_t const              pWheelStepsPerPush,
               std::uint16_t const              pWheelSpeed,
               std::normal_distribution<> const pWheelResetTime) noexcept;

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

    /// @brief The vertical cursor speed [pxl/s].
    std::normal_distribution<> _cursorSpeed;

    /// @brief The factor to get from vertical to horizontal cursor speed.
    double _horizontalSpeedFactor;

    /// @brief The steps the mouse wheel does per of the finger.
    std::uint16_t _wheelStepsPerPush;

    /// @brief The speed the wheel is turned at [steps/s].
    std::uint16_t _wheelSpeed;

    /// @brief The time it takes for the finger to reposition for the next push [ms].
    std::normal_distribution<> _wheelResetTime;
};

} // namespace Terrahertz::Input

#endif // !THZ_AUTO_GAMING_INPUT_PARAMETERES_HPP
