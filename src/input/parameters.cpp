#include "THzAutoGaming/input/parameters.hpp"

#include <cmath>

namespace Terrahertz::Input {

using ms = std::chrono::milliseconds;

std::optional<Parameters> Parameters::create(std::normal_distribution<> const pKeyDownTime,
                                             std::normal_distribution<> const pKeyUpTime,
                                             std::normal_distribution<> const pButtonDownTime,
                                             std::normal_distribution<> const pButtonUpTime,
                                             std::normal_distribution<> const pCursorAccuracy,
                                             std::normal_distribution<> const pCursorSpeedX,
                                             std::normal_distribution<> const pCursorSpeedY) noexcept
{
    auto const normalDistributionBaseCeck = [](std::normal_distribution<> const &toCeck) noexcept -> bool {
        auto const mean   = toCeck.mean();
        auto const stdDev = toCeck.stddev();
        return std::isnormal(mean) && (mean > 0.0);
    };
    if (!normalDistributionBaseCeck(pKeyDownTime) || !normalDistributionBaseCeck(pKeyUpTime) ||
        !normalDistributionBaseCeck(pButtonDownTime) || !normalDistributionBaseCeck(pButtonUpTime) ||
        !normalDistributionBaseCeck(pCursorSpeedX) || !normalDistributionBaseCeck(pCursorSpeedY))
    {
        return {};
    }
    if (pCursorAccuracy.mean() != 0.0)
    {
        return {};
    }
    return Parameters{
        pKeyDownTime, pKeyUpTime, pButtonDownTime, pButtonUpTime, pCursorAccuracy, pCursorSpeedX, pCursorSpeedY};
}

Parameters Parameters::Human() noexcept
{
    return Parameters{std::normal_distribution<>{55.0, 24.0},
                      std::normal_distribution<>{108.0, 130.0},
                      std::normal_distribution<>{77.0, 14.0},
                      std::normal_distribution<>{76.0, 21.0},
                      std::normal_distribution<>{0.0, 0.2821},
                      std::normal_distribution<>{492.65, 335.93},
                      std::normal_distribution<>{272.98, 211.43}};
}

Parameters Parameters::Fast() noexcept
{

    return Parameters{std::normal_distribution<>{50.0, 0.0001},
                      std::normal_distribution<>{50.0, 0.0001},
                      std::normal_distribution<>{50.0, 0.0001},
                      std::normal_distribution<>{50.0, 0.0001},
                      std::normal_distribution<>{0.0, 0.0001},
                      std::normal_distribution<>{3840.0, 0.0001},
                      std::normal_distribution<>{2160.0, 0.0001}};
}

Parameters::Parameters(std::normal_distribution<> const pKeyDownTime,
                       std::normal_distribution<> const pKeyUpTime,
                       std::normal_distribution<> const pButtonDownTime,
                       std::normal_distribution<> const pButtonUpTime,
                       std::normal_distribution<> const pCursorAccuracy,
                       std::normal_distribution<> const pCursorSpeedX,
                       std::normal_distribution<> const pCursorSpeedY) noexcept
    : _keyDownTime{pKeyDownTime},
      _keyUpTime{pKeyUpTime},
      _buttonDownTime{pButtonDownTime},
      _buttonUpTime{pButtonUpTime},
      _cursorAccuracy{pCursorAccuracy},
      _cursorSpeedX{pCursorSpeedX},
      _cursorSpeedY{pCursorSpeedY}
{}

} // namespace Terrahertz::Input
