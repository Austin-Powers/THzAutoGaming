#include "THzAutoGaming/input/normalDeviationStrategy.hpp"

#include <gtest/gtest.h>

namespace Terrahertz::UnitTests {

struct Input_NormalDeviationStrategy : public testing::Test
{
    Input::NormalDeviationStrategy sut{Input::Parameters::Human()};
};

TEST_F(Input_NormalDeviationStrategy, ParametersGivenAreStoredCorrectly)
{
    auto const expected = Input::Parameters::Human();

    Input::NormalDeviationStrategy instance{expected};

    auto const &actual = instance.parameters();
    EXPECT_EQ(expected.keyDownTime().mean(), actual.keyDownTime().mean());
    EXPECT_EQ(expected.keyDownTime().stddev(), actual.keyDownTime().stddev());
    EXPECT_EQ(expected.keyUpTime().mean(), actual.keyUpTime().mean());
    EXPECT_EQ(expected.keyUpTime().stddev(), actual.keyUpTime().stddev());
    EXPECT_EQ(expected.buttonDownTime().mean(), actual.buttonDownTime().mean());
    EXPECT_EQ(expected.buttonDownTime().stddev(), actual.buttonDownTime().stddev());
    EXPECT_EQ(expected.buttonUpTime().mean(), actual.buttonUpTime().mean());
    EXPECT_EQ(expected.buttonUpTime().stddev(), actual.buttonUpTime().stddev());
    EXPECT_EQ(expected.cursorAccuracy().mean(), actual.cursorAccuracy().mean());
    EXPECT_EQ(expected.cursorAccuracy().stddev(), actual.cursorAccuracy().stddev());
    EXPECT_EQ(expected.cursorSpeedX().mean(), actual.cursorSpeedX().mean());
    EXPECT_EQ(expected.cursorSpeedX().stddev(), actual.cursorSpeedX().stddev());
    EXPECT_EQ(expected.cursorSpeedY().mean(), actual.cursorSpeedY().mean());
    EXPECT_EQ(expected.cursorSpeedY().stddev(), actual.cursorSpeedY().stddev());
}

TEST_F(Input_NormalDeviationStrategy, CalculateKeyDownTime)
{
    auto const parameter = Input::Parameters::Human().keyDownTime();
    auto const maxValue  = static_cast<std::uint32_t>(parameter.mean() + (3.0 * parameter.stddev()));

    auto lastValue = sut.calculateKeyDownTime();
    EXPECT_GT(lastValue.count(), 0U);
    EXPECT_LT(lastValue.count(), maxValue);

    auto lastAndCurrentAreEqualCounter = 0U;
    for (auto i = 0U; i < 64U; ++i)
    {
        auto const currentValue = sut.calculateKeyDownTime();
        EXPECT_GT(currentValue.count(), 0U);
        EXPECT_LT(currentValue.count(), maxValue);
        if (currentValue == lastValue)
        {
            ++lastAndCurrentAreEqualCounter;
        }
        lastValue = currentValue;
    }
    EXPECT_LT(lastAndCurrentAreEqualCounter, 3U);
}

TEST_F(Input_NormalDeviationStrategy, CalculateKeyUpTime)
{
    auto const parameter = Input::Parameters::Human().keyUpTime();
    auto const maxValue  = static_cast<std::uint32_t>(parameter.mean() + (3.0 * parameter.stddev()));

    auto lastValue = sut.calculateKeyUpTime();
    EXPECT_GT(lastValue.count(), 0U);
    EXPECT_LT(lastValue.count(), maxValue);

    auto lastAndCurrentAreEqualCounter = 0U;
    for (auto i = 0U; i < 64U; ++i)
    {
        auto const currentValue = sut.calculateKeyUpTime();
        EXPECT_GT(currentValue.count(), 0U);
        EXPECT_LT(currentValue.count(), maxValue);
        if (currentValue == lastValue)
        {
            ++lastAndCurrentAreEqualCounter;
        }
        lastValue = currentValue;
    }
    EXPECT_LT(lastAndCurrentAreEqualCounter, 3U);
}

} // namespace Terrahertz::UnitTests
