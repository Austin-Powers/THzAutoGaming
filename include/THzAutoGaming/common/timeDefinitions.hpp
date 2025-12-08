#ifndef THZ_AUTOGAMING_COMMON_DEFINITIONS_HPP
#define THZ_AUTOGAMING_COMMON_DEFINITIONS_HPP

#include <chrono>

namespace Terrahertz {

/// @brief Shorthand for milliseconds.
using Ms = std::chrono::milliseconds;

/// @brief The type of clock used throughout the library.
using Clock = std::chrono::system_clock;

/// @brief The type of time_point used throughout the library.
using TimePoint = std::chrono::time_point<Clock>;

} // namespace Terrahertz

#endif // !THZ_AUTOGAMING_COMMON_DEFINITIONS_HPP
