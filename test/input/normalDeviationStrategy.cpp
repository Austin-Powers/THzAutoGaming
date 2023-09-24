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
    EXPECT_EQ(expected.cursorSpeed().mean(), actual.cursorSpeed().mean());
    EXPECT_EQ(expected.cursorSpeed().stddev(), actual.cursorSpeed().stddev());
    EXPECT_EQ(expected.horizontalSpeedFactor(), actual.horizontalSpeedFactor());
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

TEST_F(Input_NormalDeviationStrategy, CalculateButtonDownTime)
{
    auto const parameter = Input::Parameters::Human().buttonDownTime();
    auto const maxValue  = static_cast<std::uint32_t>(parameter.mean() + (3.0 * parameter.stddev()));

    auto lastValue = sut.calculateButtonDownTime();
    EXPECT_GT(lastValue.count(), 0U);
    EXPECT_LT(lastValue.count(), maxValue);

    auto lastAndCurrentAreEqualCounter = 0U;
    for (auto i = 0U; i < 64U; ++i)
    {
        auto const currentValue = sut.calculateButtonDownTime();
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

TEST_F(Input_NormalDeviationStrategy, CalculateButtonUpTime)
{
    auto const parameter = Input::Parameters::Human().buttonUpTime();
    auto const maxValue  = static_cast<std::uint32_t>(parameter.mean() + (3.0 * parameter.stddev()));

    auto lastValue = sut.calculateButtonUpTime();
    EXPECT_GT(lastValue.count(), 0U);
    EXPECT_LT(lastValue.count(), maxValue);

    auto lastAndCurrentAreEqualCounter = 0U;
    for (auto i = 0U; i < 64U; ++i)
    {
        auto const currentValue = sut.calculateButtonUpTime();
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

TEST_F(Input_NormalDeviationStrategy, CalculateTargetIn)
{
    Rectangle const targetArea{160, 90, 320, 180};

    auto lastValue = sut.calculateTargetIn(targetArea);
    EXPECT_TRUE(targetArea.encloses(lastValue));

    auto lastAndCurrentAreEqualCounter = 0U;
    for (auto i = 0U; i < 64U; ++i)
    {
        auto const currentValue = sut.calculateTargetIn(targetArea);
        EXPECT_TRUE(targetArea.encloses(currentValue));
        if (currentValue == lastValue)
        {
            ++lastAndCurrentAreEqualCounter;
        }
        lastValue = currentValue;
    }
    EXPECT_LT(lastAndCurrentAreEqualCounter, 3U);
}

TEST_F(Input_NormalDeviationStrategy, CalculateSpeed)
{
    auto const parameter = Input::Parameters::Human().cursorSpeed();
    auto const maxValue  = static_cast<std::uint32_t>(parameter.mean() + (3.0 * parameter.stddev()));

    auto lastValue = sut.calculateSpeed();
    EXPECT_GT(lastValue, 0U);
    EXPECT_LT(lastValue, maxValue);

    auto lastAndCurrentAreEqualCounter = 0U;
    for (auto i = 0U; i < 64U; ++i)
    {
        auto const currentValue = sut.calculateSpeed();
        EXPECT_GT(currentValue, 0U);
        EXPECT_LT(currentValue, maxValue);
        if (currentValue == lastValue)
        {
            ++lastAndCurrentAreEqualCounter;
        }
        lastValue = currentValue;
    }
    EXPECT_LT(lastAndCurrentAreEqualCounter, 3U);
}

TEST_F(Input_NormalDeviationStrategy, CalculateHorizontalSpeedFactor)
{
    EXPECT_EQ(Input::Parameters::Human().horizontalSpeedFactor(), sut.calculateHorizontalSpeedFactor());
}

TEST_F(Input_NormalDeviationStrategy, CalculateWheelSteps)
{
    auto const limit = Input::Parameters::Human().wheelStepsPerPush();
    EXPECT_EQ(sut.calculateWheelSteps(limit + 1), limit);
    EXPECT_EQ(sut.calculateWheelSteps(limit), limit);
    EXPECT_EQ(sut.calculateWheelSteps(limit - 1), limit - 1);
    EXPECT_EQ(sut.calculateWheelSteps(0), 0);
    EXPECT_EQ(sut.calculateWheelSteps(1 - limit), 1 - limit);
    EXPECT_EQ(sut.calculateWheelSteps(0 - limit), 0 - limit);
    EXPECT_EQ(sut.calculateWheelSteps(-1 - limit), 0 - limit);
}

TEST_F(Input_NormalDeviationStrategy, CalculateWheelSpeed)
{
    EXPECT_EQ(Input::Parameters::Human().wheelSpeed(), sut.calculateWheelSpeed());
}

TEST_F(Input_NormalDeviationStrategy, CalculateWheelResetTime)
{
    auto const parameter = Input::Parameters::Human().wheelResetTime();
    auto const maxValue  = static_cast<std::uint32_t>(parameter.mean() + (3.0 * parameter.stddev()));

    auto lastValue = sut.calculateWheelResetTime();
    EXPECT_GT(lastValue.count(), 0U);
    EXPECT_LT(lastValue.count(), maxValue);

    auto lastAndCurrentAreEqualCounter = 0U;
    for (auto i = 0U; i < 64U; ++i)
    {
        auto const currentValue = sut.calculateWheelResetTime();
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
