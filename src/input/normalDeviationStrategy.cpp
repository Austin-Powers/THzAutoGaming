#include "THzAutoGaming/input/normalDeviationStrategy.hpp"

#include <cstdint>

namespace Terrahertz::Input {

NormalDeviationStrategy::NormalDeviationStrategy(Parameters const &pParameters) noexcept : _parameters{pParameters} {}

std::chrono::milliseconds NormalDeviationStrategy::calculateKeyDownTime() noexcept
{
    auto const &distr    = _parameters.keyDownTime();
    auto const  maxValue = distr.mean() + (3.0 * distr.stddev());
    auto        result   = 0.0;
    while ((result <= 0.0) || (result >= maxValue))
    {
        result = _parameters.keyDownTime()(_generator);
    }
    return std::chrono::milliseconds{static_cast<std::uint32_t>(result)};
}

} // namespace Terrahertz::Input
