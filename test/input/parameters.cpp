#include "THzAutoGaming/input/parameters.hpp"

#include <chrono>
#include <cstdint>
#include <gtest/gtest.h>
#include <limits>

namespace Terrahertz::UnitTests {

struct Input_Parameters : public testing::Test
{
    using Time  = Input::Parameters::Time;
    using Speed = Input::Parameters::Speed;
    using ms    = std::chrono::milliseconds;

    Time   keyDownTime{ms{365}, ms{122}};
    Time   keyUpTime{ms{675}, ms{178}};
    Time   buttonDownTime{ms{221}, ms{54}};
    Time   buttonUpTime{ms{675}, ms{20}};
    double cursorAccuracy{0.4};
    Speed  cursorSpeedX{222.2, 22.5};
    Speed  cursorSpeedY{120.5, 10.6};

    template <typename T>
    void testCreationWithInvalidParameter(T &parameter, T const tempValue) noexcept
    {
        T const temp = parameter;
        parameter    = tempValue;
        EXPECT_FALSE(
            Input::Parameters::create(
                keyDownTime, keyUpTime, buttonDownTime, buttonUpTime, cursorAccuracy, cursorSpeedX, cursorSpeedY)
                .has_value());
        parameter = temp;
    }
};

TEST_F(Input_Parameters, CreationReturnsEmptyOptionalIfGivenInvalidParameters)
{
    testCreationWithInvalidParameter(keyDownTime.mean, ms{-1});
    testCreationWithInvalidParameter(keyDownTime.stddev, ms{-1});
    testCreationWithInvalidParameter(keyUpTime.mean, ms{-1});
    testCreationWithInvalidParameter(keyUpTime.stddev, ms{-1});
    testCreationWithInvalidParameter(buttonDownTime.mean, ms{-1});
    testCreationWithInvalidParameter(buttonDownTime.stddev, ms{-1});
    testCreationWithInvalidParameter(buttonUpTime.mean, ms{-1});
    testCreationWithInvalidParameter(buttonUpTime.stddev, ms{-1});

    auto const testDouble = [&](double &value, double const t0, double const t1) noexcept {
        testCreationWithInvalidParameter(value, t0);
        testCreationWithInvalidParameter(value, t1);
        testCreationWithInvalidParameter(value, std::numeric_limits<double>::quiet_NaN());
        testCreationWithInvalidParameter(value, std::numeric_limits<double>::infinity());
    };
    testDouble(cursorAccuracy, -0.0001, 1.0001);
    testDouble(cursorSpeedX.mean, -0.0001, 0.0);
    testDouble(cursorSpeedX.stddev, -0.0001, -0.0001);
    testDouble(cursorSpeedY.mean, -0.0001, 0.0);
    testDouble(cursorSpeedY.stddev, -0.0001, -0.0001);
}

TEST_F(Input_Parameters, DataStoredCorrectly)
{
    auto const sut =
        Input::Parameters::create(
            keyDownTime, keyUpTime, buttonDownTime, buttonUpTime, cursorAccuracy, cursorSpeedX, cursorSpeedY)
            .value();

    EXPECT_EQ(sut.keyDownTime().mean, keyDownTime.mean);
    EXPECT_EQ(sut.keyDownTime().stddev, keyDownTime.stddev);
    EXPECT_EQ(sut.keyUpTime().mean, keyUpTime.mean);
    EXPECT_EQ(sut.keyUpTime().stddev, keyUpTime.stddev);
    EXPECT_EQ(sut.buttonDownTime().mean, buttonDownTime.mean);
    EXPECT_EQ(sut.buttonDownTime().stddev, buttonDownTime.stddev);
    EXPECT_EQ(sut.buttonUpTime().mean, buttonUpTime.mean);
    EXPECT_EQ(sut.buttonUpTime().stddev, buttonUpTime.stddev);
    EXPECT_EQ(sut.cursorAccuracy(), cursorAccuracy);
    EXPECT_EQ(sut.cursorSpeedX().mean, cursorSpeedX.mean);
    EXPECT_EQ(sut.cursorSpeedX().stddev, cursorSpeedX.stddev);
    EXPECT_EQ(sut.cursorSpeedY().mean, cursorSpeedY.mean);
    EXPECT_EQ(sut.cursorSpeedY().stddev, cursorSpeedY.stddev);
}

TEST_F(Input_Parameters, PresetsValid)
{
    auto const checkParameters = [](Input::Parameters const &parameters) noexcept {
        EXPECT_TRUE(Input::Parameters::create(parameters.keyDownTime(),
                                              parameters.keyUpTime(),
                                              parameters.buttonDownTime(),
                                              parameters.buttonUpTime(),
                                              parameters.cursorAccuracy(),
                                              parameters.cursorSpeedX(),
                                              parameters.cursorSpeedY())
                        .has_value());
    };
    checkParameters(Input::Parameters::Human());
    checkParameters(Input::Parameters::Fast());
}

} // namespace Terrahertz::UnitTests
