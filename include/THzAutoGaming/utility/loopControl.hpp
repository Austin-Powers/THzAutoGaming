#ifndef THZ_AUTOGAMING_UTILITY_LOOPCONTROL_HPP
#define THZ_AUTOGAMING_UTILITY_LOOPCONTROL_HPP

#include "THzAutoGaming/common/timeDefinitions.hpp"
#include "THzAutoGaming/utility/iCondition.hpp"

#include <vector>

namespace Terrahertz {

/// @brief Ensures main loop is executed in set intervals and shutdown conditions are acted upon.
class LoopControl
{
public:
    /// @brief Waits until the start of the next loop.
    ///
    /// @return True if the loop shall continue, false otherwise.
    /// @remarks This method is intended to be used in a while loop.
    bool wait() noexcept;

    /// @brief Updates the interval of the loop.
    ///
    /// @param interval The new interval of the managed loop.
    /// @return If update was successful, false otherwise.
    bool updateInterval(std::chrono::milliseconds const interval) noexcept;

    /// @brief Returns the current interval of the loop control.
    ///
    /// @return The current interval.
    std::chrono::milliseconds currentInterval() const noexcept;

    /// @brief Shuts down the loop.
    void shutdown() noexcept;

    /// @brief Adds a condition that is checked at the beginning of wait(), that if met, will shutdown the loop.
    ///
    /// @param condition The condition to check each time wait() is called.
    /// @remark Any condition being met will trigger shutdown.
    void addShutdownCondition(ICondition *const condition) noexcept;

private:
    /// @brief Flag signalling if the loop is still running.
    bool _running{true};

    /// @brief The interval enforced by the loop control.
    Ms _interval{100U};

    /// @brief The next time the loop should run.
    TimePoint _nextPoint{Clock::now()};

    /// @brief List of conditions that trigger shutdown of the
    std::vector<ICondition *> _shutdownConditions{};
};

} // namespace Terrahertz

#endif // !THZ_AUTOGAMING_UTILITY_LOOPCONTROL_HPP
