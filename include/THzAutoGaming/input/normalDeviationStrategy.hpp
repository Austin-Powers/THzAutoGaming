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
    inline Parameters const &parameters() const noexcept { return _parameters; }

    /// @copydoc IDeviationStrategy::calculateKeyDownTime
    std::chrono::milliseconds calculateKeyDownTime() noexcept override;

    /// @copydoc IDeviationStrategy::calculateKeyUpTime
    std::chrono::milliseconds calculateKeyUpTime() noexcept override;

private:
    /// @brief Calculates the next value for the given distribution.
    ///
    /// @param distribution The distribution for the next value.
    /// @return The next value.
    std::uint32_t calculateValueUsing(std::normal_distribution<> const &distribution) noexcept;

    /// @brief The parameters used by this strategy.
    Parameters _parameters;

    /// @brief The pseude random number generator of the strategy.
    std::mt19937 _generator{};
};

} // namespace Terrahertz::Input
