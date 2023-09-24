#ifndef THZ_AUTO_GAMING_INPUT_NORMALDEVIATIONSTRATEGY_HPP
#define THZ_AUTO_GAMING_INPUT_NORMALDEVIATIONSTRATEGY_HPP

#include "iDeviationStrategy.hpp"
#include "parameters.hpp"

#include <chrono>

namespace Terrahertz::Input {

/// @brief Implements the IDeviationStrategy using Parameters and std::normal_distribution for devation.
class NormalDeviationStrategy : public IDeviationStrategy
{
public:
    /// @brief Initializes a new NormalDeviationStrategy using the given set of parameters.
    ///
    /// @param pParameters The parameters used by the new instance.
    NormalDeviationStrategy(Parameters const &pParameters) noexcept;

    /// @brief Returns the parameters used by this strategy.
    ///
    /// @return The parameters used by this strategy.
    [[nodiscard]] inline Parameters const &parameters() const noexcept { return _parameters; }

    /// @copydoc IDeviationStrategy::calculateKeyDownTime
    [[nodiscard]] std::chrono::milliseconds calculateKeyDownTime() noexcept override;

    /// @copydoc IDeviationStrategy::calculateKeyUpTime
    [[nodiscard]] std::chrono::milliseconds calculateKeyUpTime() noexcept override;

    /// @copydoc IDeviationStrategy::calculateButtonDownTime
    [[nodiscard]] std::chrono::milliseconds calculateButtonDownTime() noexcept override;

    /// @copydoc IDeviationStrategy::calculateButtonUpTime
    [[nodiscard]] std::chrono::milliseconds calculateButtonUpTime() noexcept override;

    /// @copydoc IDeviationStrategy::calculateTargetIn
    [[nodiscard]] Point calculateTargetIn(Rectangle const &area) noexcept override;

    /// @copydoc IDeviationStrategy::calculateSpeed
    [[nodiscard]] std::uint32_t calculateSpeed() noexcept override;

    /// @copydoc IDeviationStrategy::calculateHorizontalSpeedFactor
    [[nodiscard]] double calculateHorizontalSpeedFactor() noexcept override;

    /// @copydoc IDeviationStrategy::calculateWheelSteps
    [[nodiscard]] std::uint16_t calculateWheelSteps() noexcept override;

    /// @copydoc IDeviationStrategy::calculateWheelSpeed
    [[nodiscard]] std::uint16_t calculateWheelSpeed() noexcept override;

    /// @copydoc IDeviationStrategy::calculateWheelResetTime
    [[nodiscard]] std::chrono::milliseconds calculateWheelResetTime() noexcept override;

private:
    /// @brief Calculates the next value for the given distribution.
    ///
    /// @param distribution The distribution for the next value.
    /// @return The next value.
    [[nodiscard]] std::uint32_t calculateValueUsing(std::normal_distribution<> const &distribution) noexcept;

    /// @brief The parameters used by this strategy.
    Parameters _parameters;

    /// @brief The pseude random number generator of the strategy.
    std::mt19937 _generator{};
};

} // namespace Terrahertz::Input

#endif // !THZ_AUTO_GAMING_INPUT_NORMALDEVIATIONSTRATEGY_HPP
