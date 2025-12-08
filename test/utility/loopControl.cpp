#include "THzAutoGaming/utility/loopControl.hpp"

#include <gtest/gtest.h>

namespace Terrahertz::UnitTests {

struct Utility_LoopControl : public testing::Test
{
    std::uint32_t measureIntervalInMs(LoopControl &sut) { return 0U; }
};

TEST_F(Utility_LoopControl, DefaultIntervalCorrect)
{
    LoopControl sut{};
    EXPECT_EQ(sut.currentInterval().count(), 100U);
}

TEST_F(Utility_LoopControl, WaitReturnsTrueByDefault)
{
    LoopControl sut{};
    EXPECT_TRUE(sut.wait());
}

TEST_F(Utility_LoopControl, WaitReturnsFalseAfterShutdownWasCalled)
{
    LoopControl sut{};
    sut.shutdown();
    EXPECT_FALSE(sut.wait());
}

TEST_F(Utility_LoopControl, IntervalUpdate)
{
    LoopControl sut{};
    EXPECT_FALSE(sut.updateInterval(std::chrono::milliseconds{-12}));
    EXPECT_EQ(sut.currentInterval().count(), 100U);
    EXPECT_FALSE(sut.updateInterval(std::chrono::milliseconds{}));
    EXPECT_EQ(sut.currentInterval().count(), 100U);
    EXPECT_TRUE(sut.updateInterval(std::chrono::milliseconds{12}));
    EXPECT_EQ(sut.currentInterval().count(), 12U);
}

} // namespace Terrahertz::UnitTests
