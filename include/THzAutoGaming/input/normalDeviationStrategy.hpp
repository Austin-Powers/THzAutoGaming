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

private:
    /// @brief The parameters used by this strategy.
    Parameters _parameters;

    /// @brief The pseude random number generator of the strategy.
    std::mt19937 _generator{};
};

} // namespace Terrahertz::Input
