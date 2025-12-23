#ifndef THZ_AUTOGAMING_UTILITY_COMMONCONDITIONS_HPP
#define THZ_AUTOGAMING_UTILITY_COMMONCONDITIONS_HPP

#include "THzAutoGaming/input/emulator.hpp"
#include "THzAutoGaming/utility/iCondition.hpp"

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

} // namespace Terrahertz

#endif // !THZ_AUTOGAMING_UTILITY_COMMONCONDITIONS_HPP
