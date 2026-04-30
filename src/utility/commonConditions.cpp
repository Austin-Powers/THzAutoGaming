#include "THzAutoGaming/utility/commonConditions.hpp"

namespace Terrahertz {

CapsLockActive::CapsLockActive(Input::Emulator &emulator) noexcept : _emulator{emulator} {}

bool CapsLockActive::check() noexcept { return _emulator.isActive(Input::KeyboardLock::Caps); }

NumLockActive::NumLockActive(Input::Emulator &emulator) noexcept : _emulator{emulator} {}

bool NumLockActive::check() noexcept { return _emulator.isActive(Input::KeyboardLock::Num); }

EmulatorBusy::EmulatorBusy(Input::Emulator &emulator) noexcept : _emulator{emulator} {}

bool EmulatorBusy::check() noexcept
{
    return (_emulator.actionCountMouse() != 0U) || (_emulator.actionCountKeyboard() != 0U);
}

// TODO test CountdownOver

CountdownOver::CountdownOver(bool const initialCheckReturnValue, std::uint16_t const countdown) noexcept
    : _initalialCheckReturnValue{initialCheckReturnValue}, _countdown{countdown}
{}

void CountdownOver::reset(bool const initialCheckReturnValue, std::uint16_t const countdown) noexcept
{
    _initalialCheckReturnValue = initialCheckReturnValue;
    _countdown                 = countdown;
}

bool CountdownOver::check() noexcept
{
    if (_countdown == 0U)
    {
        return !_initalialCheckReturnValue;
    }
    --_countdown;
    return _initalialCheckReturnValue;
}

} // namespace Terrahertz
