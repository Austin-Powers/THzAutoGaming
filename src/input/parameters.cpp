#include "THzAutoGaming/input/parameters.hpp"

#include <cmath>

namespace Terrahertz::Input {

using ms = std::chrono::milliseconds;

std::optional<Parameters> Parameters::create(Time const   pKeyDownTime,
                                             Time const   pKeyUpTime,
                                             Time const   pButtonDownTime,
                                             Time const   pButtonUpTime,
                                             double const pCursorAccuracy,
                                             Speed const  pCursorSpeedX,
                                             Speed const  pCursorSpeedY) noexcept
{
    auto const normalOrZero = [](double const value) noexcept -> bool {
        return std::isnormal(value) || (value == 0.0);
    };
    auto const timeInvalid = [](Time const &time) noexcept -> bool {
        return ((time.mean.count() < 0) || (time.stddev.count() < 0));
    };
    if (timeInvalid(pKeyDownTime) || timeInvalid(pKeyUpTime) || timeInvalid(pButtonDownTime) ||
        timeInvalid(pButtonUpTime))
    {
        return {};
    }
    if (!normalOrZero(pCursorAccuracy) || (pCursorAccuracy < 0.0) || (pCursorAccuracy > 1.0))
    {
        return {};
    }
    auto const speedInvalid = [&](Speed const &speed) noexcept -> bool {
        return !std::isnormal(speed.mean) || !normalOrZero(speed.stddev) || (speed.mean <= 0.0) || (speed.stddev < 0.0);
    };
    if (speedInvalid(pCursorSpeedX) || speedInvalid(pCursorSpeedY))
    {
        return {};
    }
    return Parameters{
        pKeyDownTime, pKeyUpTime, pButtonDownTime, pButtonUpTime, pCursorAccuracy, pCursorSpeedX, pCursorSpeedY};
}

Parameters Parameters::Human() noexcept
{
    return Parameters{{ms{55}, ms{24}},
                      {ms{108}, ms{130}},
                      {ms{77}, ms{14}},
                      {ms{76}, ms{21}},
                      0.2821,
                      {492.65, 335.93},
                      {272.98, 211.43}};
}

Parameters Parameters::Fast() noexcept
{

    return Parameters{
        {ms{50}, ms{0}}, {ms{50}, ms{0}}, {ms{50}, ms{0}}, {ms{50}, ms{0}}, 0.0, {3840.0, 0.0}, {2160.0, 0.0}};
}

Parameters::Parameters(Time const   pKeyDownTime,
                       Time const   pKeyUpTime,
                       Time const   pButtonDownTime,
                       Time const   pButtonUpTime,
                       double const pCursorAccuracy,
                       Speed const  pCursorSpeedX,
                       Speed const  pCursorSpeedY) noexcept
    : _keyDownTime{pKeyDownTime},
      _keyUpTime{pKeyUpTime},
      _buttonDownTime{pButtonDownTime},
      _buttonUpTime{pButtonUpTime},
      _cursorAccuracy{pCursorAccuracy},
      _cursorSpeedX{pCursorSpeedX},
      _cursorSpeedY{pCursorSpeedY}
{}

} // namespace Terrahertz::Input
