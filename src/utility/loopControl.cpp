#include "THzAutoGaming/utility/loopControl.hpp"

#include <condition_variable>

namespace Terrahertz {

bool LoopControl::wait() noexcept
{
    if (!_shutdownConditions.empty())
    {
        for (auto &condition : _shutdownConditions)
        {
            if (condition->check())
            {
                _running = false;
                break;
            }
        }
    }
    if (_running)
    {
        auto const currentTime = Clock::now();
        while ((_nextPoint - currentTime).count() <= 0U)
        {
            _nextPoint += _interval;
        }
        std::mutex                   mutex{};
        std::unique_lock<std::mutex> lock{mutex};
        std::condition_variable      cv{};
        cv.wait_until(lock, _nextPoint);
    }
    return _running;
}

bool LoopControl::updateInterval(std::chrono::milliseconds const interval) noexcept
{
    if (interval.count() < 1U)
    {
        return false;
    }
    _interval = interval;
    return true;
}

std::chrono::milliseconds LoopControl::currentInterval() const noexcept { return _interval; }

void LoopControl::shutdown() noexcept { _running = false; }

void LoopControl::addShutdownCondition(ICondition *const condition) noexcept
{
    if (condition != nullptr)
    {
        _shutdownConditions.emplace_back(condition);
    }
}

} // namespace Terrahertz
