#include "THzAutoGaming/utility/commonConditions.hpp"

#include <gtest/gtest.h>

namespace Terrahertz::UnitTests {

struct UtilityCountDownOver : public testing::Test
{};

TEST_F(UtilityCountDownOver, StartingWithZero)
{
    CountdownOver sutTrue(true, 0U);
    CountdownOver sutFalse(false, 0U);
    EXPECT_FALSE(sutTrue.check());
    EXPECT_FALSE(sutTrue.check());
    EXPECT_TRUE(sutFalse.check());
    EXPECT_TRUE(sutFalse.check());
}

TEST_F(UtilityCountDownOver, CountingDown)
{
    CountdownOver sutTrue(true, 2U);
    EXPECT_TRUE(sutTrue.check());
    EXPECT_TRUE(sutTrue.check());
    EXPECT_FALSE(sutTrue.check());
    EXPECT_FALSE(sutTrue.check());

    CountdownOver sutFalse(false, 2U);
    EXPECT_FALSE(sutFalse.check());
    EXPECT_FALSE(sutFalse.check());
    EXPECT_TRUE(sutFalse.check());
    EXPECT_TRUE(sutFalse.check());
}

TEST_F(UtilityCountDownOver, Reset)
{
    CountdownOver sut(true, 2U);
    EXPECT_TRUE(sut.check());
    sut.reset(false, 1U);
    EXPECT_FALSE(sut.check());
    EXPECT_TRUE(sut.check());
    sut.reset(true, 2U);
    EXPECT_TRUE(sut.check());
    EXPECT_TRUE(sut.check());
    EXPECT_FALSE(sut.check());
}

} // namespace Terrahertz::UnitTests
