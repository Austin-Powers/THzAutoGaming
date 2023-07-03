#include "THzAutoGaming/input/parameters.hpp"

#include <cstdint>
#include <gtest/gtest.h>

namespace Terrahertz::UnitTests {

struct Input_Parameters : public testing::Test
{};

TEST_F(Input_Parameters, Data_Stored_Correctly)
{
    std::uint32_t const keyDownTimeMean{365U};
    std::uint32_t const keyDownTimeStdDev{112U};
    std::uint32_t const keyUpTimeMean{675U};
    std::uint32_t const keyUpTimeStdDev{92U};
    std::uint32_t const buttonDownTimeMean{221U};
    std::uint32_t const buttonDownTimeStdDev{54U};
    std::uint32_t const buttonUpTimeMean{178U};
    std::uint32_t const buttonUpTimeStdDev{20U};

    double const cursorAccuracy{0.4};
    double const cursorSpeedXMean{222.2};
    double const cursorSpeedXStdDev{22.5};
    double const cursorSpeedYMean{120.5};
    double const cursorSpeedYStdDev{10.6};

    Input::Parameters const sut{keyDownTimeMean,
                                keyDownTimeStdDev,
                                keyUpTimeMean,
                                keyUpTimeStdDev,
                                buttonDownTimeMean,
                                buttonDownTimeStdDev,
                                buttonUpTimeMean,
                                buttonUpTimeStdDev,
                                cursorAccuracy,
                                cursorSpeedXMean,
                                cursorSpeedXStdDev,
                                cursorSpeedYMean,
                                cursorSpeedYStdDev};

    EXPECT_EQ(sut.keyDownTime.mean.count(), keyDownTimeMean);
    EXPECT_EQ(sut.keyDownTime.stddev.count(), keyDownTimeStdDev);
    EXPECT_EQ(sut.keyUpTime.mean.count(), keyUpTimeMean);
    EXPECT_EQ(sut.keyUpTime.stddev.count(), keyUpTimeStdDev);
    EXPECT_EQ(sut.buttonDownTime.mean.count(), buttonDownTimeMean);
    EXPECT_EQ(sut.buttonDownTime.stddev.count(), buttonDownTimeStdDev);
    EXPECT_EQ(sut.buttonUpTime.mean.count(), buttonUpTimeMean);
    EXPECT_EQ(sut.buttonUpTime.stddev.count(), buttonUpTimeStdDev);
    EXPECT_EQ(sut.cursorAccuracy, cursorAccuracy);
    EXPECT_EQ(sut.cursorSpeedX.mean, cursorSpeedXMean);
    EXPECT_EQ(sut.cursorSpeedX.stddev, cursorSpeedXStdDev);
    EXPECT_EQ(sut.cursorSpeedY.mean, cursorSpeedYMean);
    EXPECT_EQ(sut.cursorSpeedY.stddev, cursorSpeedYStdDev);
}

} // namespace Terrahertz::UnitTests
