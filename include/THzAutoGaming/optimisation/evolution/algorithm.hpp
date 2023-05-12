#ifndef THZ_AUTO_GAMING_OPTIMISATION_EVOLUTION_ALGORITHM_HPP
#define THZ_AUTO_GAMING_OPTIMISATION_EVOLUTION_ALGORITHM_HPP

#include <algorithm>
#include <cmath>
#include <concepts>
#include <cstddef>
#include <cstdint>
#include <fstream>
#include <random>
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

    {evaluator(individual)} -> std::same_as<double>;
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
    {
        resizePopulation();
    }

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
            if (toCheck == 0.0)
            {
                // 0.0 is allowed by this but not normal
                return false;
            }
            if (!std::isnormal(toCheck))
            {
                return true;
            }
            return toCheck < 0.0;
        };
        if ((newParameters.population == 0U) || (newParameters.survivors < 2U))
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
        resizePopulation();
        return true;
    }

    bool load(std::ifstream &file) noexcept { return false; }

    bool save(std::ofstream &file) noexcept { return false; }

    /// @brief Runs the evolution for one generation.
    void runOnce() noexcept
    {
        for (auto &i : _population)
        {
            if (i.state == Individual::State::Empty)
            {
                fillIndividual(i);
            }
            i.fitness = _evaluator(i.individual);
        }

        std::uniform_int_distribution<size_t> dist{0U, _population.size() - 1U};
        for (auto i = _parameters.population - _parameters.survivors; i > 0; --i)
        {
            auto idxA = dist(_rng);
            while (_population[idxA].state == Individual::State::Empty)
            {
                ++idxA;
                if (idxA == _population.size())
                {
                    idxA = 0U;
                }
            }
            auto idxB = dist(_rng);
            while ((_population[idxB].state == Individual::State::Empty) || (idxA == idxB))
            {
                ++idxB;
                if (idxB == _population.size())
                {
                    idxB = 0U;
                }
            }
            if (_population[idxA].fitness > _population[idxB].fitness)
            {
                _population[idxB].state = Individual::State::Empty;
            }
            else
            {
                _population[idxA].state = Individual::State::Empty;
            }
        }
        ++_generation;
    }

    template <typename TFunctor>
    size_t runUntil(TFunctor const &predicate) noexcept
    {
        return 0U;
    }

    /// @brief Returns the current generation of the evolution.
    ///
    /// @return The current generation of the evolution.
    size_t generation() const noexcept { return _generation; }

    TIndividualType const &bestIndividual() const noexcept {}

private:
    /// @brief Structure to augment the TIndividualType with additional data.
    struct Individual
    {
        /// @brief Enumeration of the state of an individual.
        enum class State
        {
            // Individual is empty
            Empty,

            // Individual was created by calling init
            Init,

            // Individual was created by calling reproduce
            Reproduction,

            // Individual was created by calling mutate
            Mutation
        };

        /// @brief The state of the individual.
        State state{};

        /// @brief The fitness of the individual.
        double fitness{};

        /// @brief The individual to augment.
        TIndividualType individual{};

        /// @brief Initializes a new Individual using the given root.
        ///
        /// @param root The root individual to copy from.
        Individual(TIndividualType const &root) noexcept : individual{root} {}
    };

    /// @brief Resizes the population, keeping all current individual in case it is shrinking.
    void resizePopulation() noexcept
    {
        if (_population.size() < _parameters.population)
        {
            _population.reserve(_parameters.population);
        }

        while (_population.size() < _parameters.population)
        {
            _population.emplace_back(_rootIndividual);
        }

        if (_population.size() > _parameters.population)
        {
            _population.erase(
                std::remove_if(_population.begin(), _population.end(), [](Individual const &i) noexcept -> bool {
                    return i.state == Individual::State::Empty;
                }));
        }
    }

    void fillIndividual(Individual &indivual) noexcept
    {
        if (_generation == 0U)
        {
            indivual.state = Individual::State::Init;
            indivual.individual.init();
        }
    };

    /// @brief The parameters of the algorithm run.
    Parameters _parameters{};

    /// @brief The root individual, all other individuals are copied from.
    TIndividualType _rootIndividual;

    /// @brief The evaluator
    TEvaluatorType _evaluator;

    /// @brief The population to evolve.
    std::vector<Individual> _population{};

    /// @brief The random number generator used for filling the grid.
    std::default_random_engine _rng{};

    /// @brief The current generation.
    size_t _generation{};
};

} // namespace Terrahertz::Optimisation::Evolution

#endif // !THZ_AUTO_GAMING_OPTIMISATION_EVOLUTION_ALGORITHM_HPP
