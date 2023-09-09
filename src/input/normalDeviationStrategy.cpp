#include "THzAutoGaming/input/normalDeviationStrategy.hpp"

#include <cstdint>

namespace Terrahertz::Input {

NormalDeviationStrategy::NormalDeviationStrategy(Parameters const &pParameters) noexcept : _parameters{pParameters} {}

std::chrono::milliseconds NormalDeviationStrategy::calculateKeyDownTime() noexcept
{
    return std::chrono::milliseconds{calculateValueUsing(_parameters.keyDownTime())};
}

std::chrono::milliseconds NormalDeviationStrategy::calculateKeyUpTime() noexcept
{
    return std::chrono::milliseconds{calculateValueUsing(_parameters.keyUpTime())};
}

std::chrono::milliseconds NormalDeviationStrategy::calculateButtonDownTime() noexcept
{
    return std::chrono::milliseconds{calculateValueUsing(_parameters.buttonDownTime())};
}

std::chrono::milliseconds NormalDeviationStrategy::calculateButtonUpTime() noexcept
{
    return std::chrono::milliseconds{calculateValueUsing(_parameters.buttonUpTime())};
}

Point NormalDeviationStrategy::calculateTargetIn(Rectangle const &area) noexcept
{
    auto target = area.center();
    do
    {
        target.x += static_cast<std::int32_t>(_parameters.cursorAccuracy()(_generator) * area.width);
        target.y += static_cast<std::int32_t>(_parameters.cursorAccuracy()(_generator) * area.height);
    } while (!area.encloses(target));
    return target;
}

std::uint32_t NormalDeviationStrategy::calculateValueUsing(std::normal_distribution<> const &distribution) noexcept
{
    auto const maxValue = distribution.mean() + (3.0 * distribution.stddev());

    auto result = 0.0;
    while ((result <= 0.0) || (result >= maxValue))
    {
        result = _parameters.keyDownTime()(_generator);
    }
    return static_cast<std::uint32_t>(result);
}

} // namespace Terrahertz::Input
