#include "THzAutoGaming/utility/loopControl.hpp"

#include <chrono>
#include <gtest/gtest.h>

using std::chrono::duration_cast;
using ms = std::chrono::milliseconds;

#define NOW_MS duration_cast<ms>(std::chrono::system_clock::now().time_since_epoch()).count()

namespace Terrahertz::UnitTests {

class MockCondition : public ICondition
{
public:
    std::uint8_t returnTrueFor = 0U;

    std::uint8_t timesCalled = 0U;

    bool check() noexcept override
    {
        ++timesCalled;
        if (returnTrueFor > 0U)
        {
            --returnTrueFor;
            return true;
        }
        return false;
    }
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
    sut.addSkipCondition(nullptr);
    EXPECT_TRUE(sut.wait());
    sut.shutdown();
    EXPECT_FALSE(sut.wait());
}

TEST_F(UtilityLoopControl, AddingShutdownCondition)
{
    LoopControl   sut{};
    MockCondition condition{};

    EXPECT_TRUE(sut.updateInterval(Ms{10}));
    sut.addShutdownCondition(&condition);
    EXPECT_TRUE(sut.wait());
    EXPECT_EQ(condition.timesCalled, 1U);
    condition.returnTrueFor = 1U;
    EXPECT_FALSE(sut.wait());
    EXPECT_EQ(condition.timesCalled, 2U);
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
    condition1.returnTrueFor = 1U;
    EXPECT_FALSE(sut.wait());
}

TEST_F(UtilityLoopControl, AddingSkipCondition)
{
    LoopControl   sut{};
    MockCondition condition{};

    EXPECT_TRUE(sut.updateInterval(Ms{40}));
    sut.addSkipCondition(&condition);

    auto startTime = NOW_MS;
    EXPECT_TRUE(sut.wait());
    auto const duration0 = NOW_MS - startTime;
    EXPECT_EQ(condition.timesCalled, 1U);
    EXPECT_GE(duration0, 35U);
    EXPECT_LE(duration0, 45U);

    condition.returnTrueFor = 2U;

    startTime = NOW_MS;
    EXPECT_TRUE(sut.wait());
    auto const duration1 = NOW_MS - startTime;
    EXPECT_EQ(condition.timesCalled, 4U);
    EXPECT_GE(duration1, 110U);
    EXPECT_LE(duration1, 150U);
}

TEST_F(UtilityLoopControl, OneOfManyConditionsIsEnoughToSkip)
{
    LoopControl   sut{};
    MockCondition condition0{};
    MockCondition condition1{};
    sut.addSkipCondition(&condition0);
    sut.addSkipCondition(&condition1);
    condition0.returnTrueFor = 1U;

    EXPECT_TRUE(sut.updateInterval(Ms{40}));

    auto const startTime = NOW_MS;
    EXPECT_TRUE(sut.wait());
    auto const duration = NOW_MS - startTime;
    EXPECT_GE(duration, 70U);
    EXPECT_LE(duration, 90U);
}

} // namespace Terrahertz::UnitTests
