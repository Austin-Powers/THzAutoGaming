#ifndef THZ_AUTO_GAMING_OPTIMISATION_EVOLUTION_ALGORITHM_HPP
#define THZ_AUTO_GAMING_OPTIMISATION_EVOLUTION_ALGORITHM_HPP

#include <concepts>
#include <cstddef>
#include <cstdint>
#include <fstream>
#include <limits>
#include <type_traits>
#include <vector>

namespace Terrahertz::Optimisation::Evolution {

// clang-format off

/// @brief Concept for an individual of the population for the evolutionary algorithm.
template<typename TIndividualType>
concept Individual = requires(TIndividualType individual, TIndividualType const cIndividual, std::ifstream iFile, std::ofstream oFile)
{
    // copy constructible
    TIndividualType(cIndividual);

    // copy assignable
    individual = cIndividual;

	// method for initializing the individual with new values from scrath
	{individual.init()};

	// method for initializing the individual with values from combining two existing individuals
	{individual.reproduce(cIndividual, cIndividual)};
	
	// method for initializing the individual with slightly altered values from one existing individual
	{individual.mutate(cIndividual)};

	// method for saving the state of the individual to a std::ofstream
	{cIndividual.save(oFile)} -> std::same_as<bool>;

	// method for loading the state of the individual to a std::ifstream
	{individual.load(iFile)} -> std::same_as<bool>;
};

/// @brief Concept for a evaluator for individuals.
template<typename TEvaluatorType, typename TIndividualType>
concept Evaluator = requires(TEvaluatorType evaluator, TEvaluatorType const cEvaluator, TIndividualType const individual)
{
    // copy constructible
    TEvaluatorType(cEvaluator);

    // copy assignable
    evaluator = cEvaluator;

    {evaluator.evaluate(individual)} -> std::same_as<double>;
};

// clang-format on

/// @brief Structure containing the parameters for the evolution run.
struct Parameters
{
    /// @brief The number of individuals in the population.
    std::uint32_t population{100U};

    /// @brief The number of individuals left at the end of a generation.
    std::uint32_t survivors{30};

    /// @brief The portion of the population that gets refilled via reproduction.
    ///
    /// @remark The actual ratios are determined by dividing this value by the sum of all portions.
    double reproductionPortion{0.4};

    /// @brief The portion of the population that gets refilled via mutation.
    ///
    /// @remark The actual ratios are determined by dividing this value by the sum of all portions.
    double mutationPortion{0.4};

    /// @brief The portion of the population that gets refilled via reinitialisation.
    ///
    /// @remark The actual ratios are determined by dividing this value by the sum of all portions.
    double reinitPortion{0.2};

    /// @brief Factor by which the ratios change due to the performance of certain refill strategies, 0.0 to fix ratios.
    ///
    /// @remark Ratios will change based on how many individuals created each methods have been sorted out.
    double ratioDynamics{0.01};
};

/// @brief Tempalte class encapsulating the evolutionary algorithm.
template <Individual TIndividualType, Evaluator<TIndividualType> TEvaluatorType>
class Algorithm
{
public:
    /// @brief Initializes a new Algorithm instance.
    ///
    /// @param rootIndividual The individual all others will be copied from, used for handing in external information.
    /// @param evaluator The evaluator instance used by the algorithm.
    Algorithm(TIndividualType rootIndividual = {}, TEvaluatorType evaluator = {}) noexcept
        : _rootIndividual{rootIndividual}, _evaluator{evaluator}
    {}

    /// @brief Returns the parameters of the algorithm.
    ///
    /// @return The current parameters of the algorithm.
    Parameters const &parameters() const noexcept { return _parameters; }

    /// @brief Sets the parameters of the algorithm.
    ///
    /// @param parameters The new parameters for the algorithms to use.
    bool setParameters(Parameters const &newParameters) noexcept
    {
        auto const wrong = [](double const toCheck) noexcept -> bool {
            if (toCheck == std::numeric_limits<double>::infinity())
            {
                return true;
            }
            if (toCheck < 0.0)
            {
                return true;
            }
            if (toCheck != toCheck)
            {
                // this check for NaN
                return true;
            }
            return false;
        };
        if ((newParameters.population == 0U) || (newParameters.survivors == 0U))
        {
            return false;
        }
        if (newParameters.population <= newParameters.survivors)
        {
            return false;
        }
        if (wrong(newParameters.reproductionPortion) || wrong(newParameters.mutationPortion) ||
            wrong(newParameters.reinitPortion) || wrong(newParameters.ratioDynamics))
        {
            return false;
        }
        _parameters = newParameters;
        return true;
    }

    bool load(std::ifstream &file) noexcept { return false; }

    bool save(std::ofstream &file) noexcept { return false; }

    bool runOnce() noexcept { return false; }

    size_t runFor(size_t overallGenerations) noexcept { return 0U; }

    template <typename TFunctor>
    size_t runUntil(TFunctor const &predicate) noexcept
    {
        return 0U;
    }

    TIndividualType const &bestIndividual() const noexcept {}

private:
    /// @brief The parameters of the algorithm run.
    Parameters _parameters{};

    /// @brief The root individual, all other individuals are copied from.
    TIndividualType _rootIndividual;

    /// @brief The evaluator
    TEvaluatorType _evaluator;

    /// @brief The population to evolve.
    std::vector<TIndividualType> _population{};
};

} // namespace Terrahertz::Optimisation::Evolution

#endif // !THZ_AUTO_GAMING_OPTIMISATION_EVOLUTION_ALGORITHM_HPP
