#include "THzAutoGaming/input/parameters.hpp"

#include <cmath>

namespace Terrahertz::Input {

using ms = std::chrono::milliseconds;

std::optional<Parameters> Parameters::create(std::normal_distribution<> const pKeyDownTime,
                                             std::normal_distribution<> const pKeyUpTime,
                                             std::normal_distribution<> const pButtonDownTime,
                                             std::normal_distribution<> const pButtonUpTime,
                                             std::normal_distribution<> const pCursorAccuracy,
                                             std::normal_distribution<> const pCursorSpeed,
                                             double const                     pHorizontalSpeedFactor) noexcept
{
    auto const normalDistributionBaseCeck = [](std::normal_distribution<> const &toCeck) noexcept -> bool {
        auto const mean   = toCeck.mean();
        auto const stdDev = toCeck.stddev();
        return std::isnormal(mean) && (mean > 0.0);
    };
    if (!normalDistributionBaseCeck(pKeyDownTime) || !normalDistributionBaseCeck(pKeyUpTime) ||
        !normalDistributionBaseCeck(pButtonDownTime) || !normalDistributionBaseCeck(pButtonUpTime) ||
        !normalDistributionBaseCeck(pCursorSpeed))
    {
        return {};
    }
    if (pCursorAccuracy.mean() != 0.0)
    {
        return {};
    }
    if ((pHorizontalSpeedFactor <= 0.0) || !std::isnormal(pHorizontalSpeedFactor))
    {
        return {};
    }
    return Parameters{pKeyDownTime,
                      pKeyUpTime,
                      pButtonDownTime,
                      pButtonUpTime,
                      pCursorAccuracy,
                      pCursorSpeed,
                      pHorizontalSpeedFactor};
}

Parameters Parameters::Human() noexcept
{
    return Parameters{std::normal_distribution<>{55.0, 24.0},
                      std::normal_distribution<>{108.0, 130.0},
                      std::normal_distribution<>{77.0, 14.0},
                      std::normal_distribution<>{76.0, 21.0},
                      std::normal_distribution<>{0.0, 0.2821},
                      std::normal_distribution<>{379.44, 187.39},
                      1.8};
}

Parameters Parameters::Fast() noexcept
{

    return Parameters{std::normal_distribution<>{50.0, 0.0001},
                      std::normal_distribution<>{50.0, 0.0001},
                      std::normal_distribution<>{50.0, 0.0001},
                      std::normal_distribution<>{50.0, 0.0001},
                      std::normal_distribution<>{0.0, 0.0001},
                      std::normal_distribution<>{2160.0, 0.0001},
                      2.0};
}

Parameters::Parameters(std::normal_distribution<> const pKeyDownTime,
                       std::normal_distribution<> const pKeyUpTime,
                       std::normal_distribution<> const pButtonDownTime,
                       std::normal_distribution<> const pButtonUpTime,
                       std::normal_distribution<> const pCursorAccuracy,
                       std::normal_distribution<> const pCursorSpeed,
                       double const                     pHorizontalSpeedFactor) noexcept
    : _keyDownTime{pKeyDownTime},
      _keyUpTime{pKeyUpTime},
      _buttonDownTime{pButtonDownTime},
      _buttonUpTime{pButtonUpTime},
      _cursorAccuracy{pCursorAccuracy},
      _cursorSpeed{pCursorSpeed},
      _horizontalSpeedFactor{pHorizontalSpeedFactor}
{}

} // namespace Terrahertz::Input
