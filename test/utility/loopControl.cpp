#include "THzAutoGaming/utility/loopControl.hpp"

#include <gtest/gtest.h>

namespace Terrahertz::UnitTests {

class MockCondition : public ICondition
{
public:
    bool returnValue = false;

    bool check() noexcept override { return returnValue; }
};

struct UtilityLoopControl : public testing::Test
{
    std::uint32_t measureIntervalInMs(LoopControl &sut) { return 0U; }
};

TEST_F(UtilityLoopControl, DefaultIntervalCorrect)
{
    LoopControl sut{};
    EXPECT_EQ(sut.currentInterval().count(), 100U);
}

TEST_F(UtilityLoopControl, WaitReturnsTrueByDefault)
{
    LoopControl sut{};
    EXPECT_TRUE(sut.wait());
}

TEST_F(UtilityLoopControl, WaitReturnsFalseAfterShutdownWasCalled)
{
    LoopControl sut{};
    sut.shutdown();
    EXPECT_FALSE(sut.wait());
}

TEST_F(UtilityLoopControl, IntervalUpdate)
{
    LoopControl sut{};
    EXPECT_FALSE(sut.updateInterval(Ms{-12}));
    EXPECT_EQ(sut.currentInterval().count(), 100U);
    EXPECT_FALSE(sut.updateInterval(Ms{}));
    EXPECT_EQ(sut.currentInterval().count(), 100U);
    EXPECT_TRUE(sut.updateInterval(Ms{12}));
    EXPECT_EQ(sut.currentInterval().count(), 12U);
}

TEST_F(UtilityLoopControl, IntervalMostlyCorrect)
{
    LoopControl sut{};
    EXPECT_TRUE(sut.updateInterval(Ms{10}));

    auto const startTime = std::chrono::steady_clock::now();

    auto i = 0U;
    while (sut.wait() && (i < 20U))
    {
        ++i;
        if (i == 10)
        {
            sut.shutdown();
        }
    }
    auto const endTime  = std::chrono::steady_clock::now();
    auto const duration = std::chrono::duration_cast<Ms>(endTime - startTime);
    EXPECT_GE(duration.count(), 95U);
    EXPECT_LE(duration.count(), 105U);
    EXPECT_EQ(i, 10U);
}

TEST_F(UtilityLoopControl, AddingNullptrConditionDoesNotCauseProblems)
{
    LoopControl sut{};
    EXPECT_TRUE(sut.updateInterval(Ms{10}));
    sut.addShutdownCondition(nullptr);
    EXPECT_TRUE(sut.wait());
    sut.shutdown();
    EXPECT_FALSE(sut.wait());
}

TEST_F(UtilityLoopControl, AddingCondition)
{
    LoopControl   sut{};
    MockCondition condition{};

    EXPECT_TRUE(sut.updateInterval(Ms{10}));
    sut.addShutdownCondition(&condition);
    EXPECT_TRUE(sut.wait());
    condition.returnValue = true;
    EXPECT_FALSE(sut.wait());
}

TEST_F(UtilityLoopControl, OneOfManyConditionsIsEnoughToShutdown)
{
    LoopControl   sut{};
    MockCondition condition0{};
    MockCondition condition1{};

    EXPECT_TRUE(sut.updateInterval(Ms{10}));
    sut.addShutdownCondition(&condition0);
    sut.addShutdownCondition(&condition1);
    EXPECT_TRUE(sut.wait());
    condition1.returnValue = true;
    EXPECT_FALSE(sut.wait());
}

} // namespace Terrahertz::UnitTests
