#include "THzAutoGaming/utility/commonConditions.hpp"

namespace Terrahertz {

CapsLockActive::CapsLockActive(Input::Emulator &emulator) noexcept : _emulator{emulator} {}

bool CapsLockActive::check() noexcept { return _emulator.isActive(Input::KeyboardLock::Caps); }

NumLockActive::NumLockActive(Input::Emulator &emulator) noexcept : _emulator{emulator} {}

bool NumLockActive::check() noexcept { return _emulator.isActive(Input::KeyboardLock::Num); }

} // namespace Terrahertz
