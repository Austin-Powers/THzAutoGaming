#ifndef THZ_AUTOGAMING_UTILITY_COMMONCONDITIONS_HPP
#define THZ_AUTOGAMING_UTILITY_COMMONCONDITIONS_HPP

#include "THzAutoGaming/input/emulator.hpp"
#include "THzAutoGaming/utility/iCondition.hpp"

#include <cstdint>

namespace Terrahertz {

/// @brief Condition checking if Caps-Lock is active.
class CapsLockActive : public ICondition
{
public:
    /// @brief Initializes a new CapsLockActive condition.
    ///
    /// @param emulator The emulator to check for the condition.
    CapsLockActive(Input::Emulator &emulator) noexcept;

    /// @brief Checks if Caps-Lock is active.
    ///
    /// @return True if Caps-Lock is active, false otherwise.
    bool check() noexcept override;

private:
    /// @brief The emulator to check for the condition.
    Input::Emulator &_emulator;
};

/// @brief Condition checking if Num-Lock is active.
class NumLockActive : public ICondition
{
public:
    /// @brief Initializes a new NumLockActive condition.
    ///
    /// @param emulator The emulator to check for the condition.
    NumLockActive(Input::Emulator &emulator) noexcept;

    /// @brief Checks if Num-Lock is active.
    ///
    /// @return True if Num-Lock is active, false otherwise.
    bool check() noexcept override;

private:
    /// @brief The emulator to check for the condition.
    Input::Emulator &_emulator;
};

/// @brief Condition checking if the emulator is busy.
class EmulatorBusy : public ICondition
{
public:
    /// @brief Initializes a new EmulatorBusy condition.
    ///
    /// @param emulator The emulator to check for the condition.
    EmulatorBusy(Input::Emulator &emulator) noexcept;

    /// @brief Checks if the emulator is busy (has actions queued up).
    ///
    /// @return True if the emulator is busy, false otherwise.
    bool check() noexcept override;

private:
    /// @brief The emulator to check for the condition.
    Input::Emulator &_emulator;
};

/// @brief Flips the current return value of check() after a certain amount of calls to check() have passed.
class CountdownOver : public ICondition
{
public:
    /// @brief Initializes a new CountdownOver condition.
    ///
    /// @param initialCheckReturnValue The initial return value of check().
    /// @param countdown The number of calls to check() after which the return value flips.
    CountdownOver(bool const initialCheckReturnValue, std::uint16_t const countdown) noexcept;

    /// @brief Resets the countdown.
    ///
    /// @param initialCheckReturnValue The initial return value of check().
    /// @param countdown The number of calls to check() after which the return value flips.
    void reset(bool const initialCheckReturnValue, std::uint16_t const countdown) noexcept;

    /// @brief Counts down and returns the appropriate result.
    ///
    /// @return The value given on reset before the countdown has been reached, the opposite afterwards.
    bool check() noexcept override;

private:
    /// @brief The return value
    bool _initalialCheckReturnValue{};

    /// @brief The current state of the countdown.
    std::uint16_t _countdown{};
};

} // namespace Terrahertz

#endif // !THZ_AUTOGAMING_UTILITY_COMMONCONDITIONS_HPP
