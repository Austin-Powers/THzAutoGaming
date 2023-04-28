#include "THzAutoGaming/optimisation/evolution/algorithm.hpp"

#include <gtest/gtest.h>
#include <limits>

namespace Terrahertz::UnitTests {

/// @brief Implementor of the Individual concept for testing.
struct TestIndiviual
{
    TestIndiviual() noexcept = default;

    TestIndiviual(size_t &counter) noexcept : _counter{&counter} {}

    TestIndiviual(TestIndiviual const &other) noexcept : _counter{other._counter}
    {
        if (_counter != nullptr)
        {
            ++(*_counter);
        }
    }

    TestIndiviual &operator=(TestIndiviual const &other) noexcept
    {
        _counter = other._counter;
        if (_counter != nullptr)
        {
            ++(*_counter);
        }
    }

    void init() noexcept {}

    void reproduce(TestIndiviual const &parentA, TestIndiviual const &parentB) noexcept {}

    void mutate(TestIndiviual const &parent) noexcept {}

    bool save(std::ofstream &file) const noexcept { return false; }

    bool load(std::ifstream &file) noexcept { return false; }

    size_t *_counter{};
};

struct TestEvaluator
{
    double evaluate(TestIndiviual const &individual) noexcept { return 0.0; }
};

struct Optimisation_Evolution_Algorithm : public testing::Test
{
    using Parameters = Optimisation::Evolution::Parameters;

    size_t individualCount{};

    TestIndiviual rootIndividual{individualCount};

    TestEvaluator evaluator{};

    Optimisation::Evolution::Algorithm<TestIndiviual, TestEvaluator> sut{rootIndividual, evaluator};

    void checkParameters(Parameters const &expected, Parameters const &actual) noexcept
    {
        EXPECT_EQ(expected.population, actual.population);
        EXPECT_EQ(expected.survivors, actual.survivors);
        EXPECT_EQ(expected.reproductionPortion, actual.reproductionPortion);
        EXPECT_EQ(expected.mutationPortion, actual.mutationPortion);
        EXPECT_EQ(expected.reinitPortion, actual.reinitPortion);
        EXPECT_EQ(expected.ratioDynamics, actual.ratioDynamics);
    }
};

TEST_F(Optimisation_Evolution_Algorithm, ParametersDefaultValues)
{
    Parameters const defaultParams{};
    EXPECT_GE(defaultParams.population, 100U);
    EXPECT_LT(defaultParams.survivors, defaultParams.population);
    EXPECT_GT(defaultParams.survivors, 0U);
    EXPECT_GT(defaultParams.reproductionPortion, 0.0);
    EXPECT_LT(defaultParams.reproductionPortion, 1.0);
    EXPECT_GT(defaultParams.mutationPortion, 0.0);
    EXPECT_LT(defaultParams.mutationPortion, 1.0);
    EXPECT_GT(defaultParams.reinitPortion, 0.0);
    EXPECT_LT(defaultParams.reinitPortion, 1.0);
    EXPECT_GT(defaultParams.ratioDynamics, 0.0);
    EXPECT_LT(defaultParams.ratioDynamics, 1.0);
}

TEST_F(Optimisation_Evolution_Algorithm, ConstructionCorrect)
{
    Parameters const defaultParams{};
    checkParameters(defaultParams, sut.parameters());

    // check if individual was only copied into the
    EXPECT_LE(individualCount, 2U);
}

TEST_F(Optimisation_Evolution_Algorithm, SettingIllegalParametersReturnsFalseAndChangesNothing)
{
    auto const initialState = sut.parameters();
    auto const checkSet     = [&](Parameters const &p) noexcept {
        EXPECT_FALSE(sut.setParameters(p));
        checkParameters(initialState, sut.parameters());
    };

    Parameters parameters{};
    // population zero
    parameters.population = 0U;
    checkSet(parameters);
    parameters.population = 500U;

    // survivors more than population
    parameters.survivors = 1000U;
    checkSet(parameters);

    // survivors zero
    parameters.survivors = 0U;
    checkSet(parameters);
    parameters.survivors = 200U;

    // mutation illegal values
    parameters.mutationPortion = std::numeric_limits<double>::infinity();
    checkSet(parameters);
    parameters.mutationPortion = std::numeric_limits<double>::quiet_NaN();
    checkSet(parameters);
    parameters.mutationPortion = -1.0;
    checkSet(parameters);
    parameters.mutationPortion = 0.4;

    // reproduction illegal values
    parameters.reproductionPortion = std::numeric_limits<double>::infinity();
    checkSet(parameters);
    parameters.reproductionPortion = std::numeric_limits<double>::quiet_NaN();
    checkSet(parameters);
    parameters.reproductionPortion = -1.0;
    checkSet(parameters);
    parameters.reproductionPortion = 0.4;

    // reinitPortion illegal values
    parameters.reinitPortion = std::numeric_limits<double>::infinity();
    checkSet(parameters);
    parameters.reinitPortion = std::numeric_limits<double>::quiet_NaN();
    checkSet(parameters);
    parameters.reinitPortion = -1.0;
    checkSet(parameters);
    parameters.reinitPortion = 0.4;

    // ratioDynamics illegal values
    parameters.ratioDynamics = std::numeric_limits<double>::infinity();
    checkSet(parameters);
    parameters.ratioDynamics = std::numeric_limits<double>::quiet_NaN();
    checkSet(parameters);
    parameters.ratioDynamics = -1.0;
    checkSet(parameters);
    parameters.ratioDynamics = 0.02;
}

TEST_F(Optimisation_Evolution_Algorithm, SettingCorrectParametersUpdatesValues)
{
    Parameters parameters{};
    EXPECT_TRUE(sut.setParameters(parameters));
}

} // namespace Terrahertz::UnitTests
