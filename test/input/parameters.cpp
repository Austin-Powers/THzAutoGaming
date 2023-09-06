#include "THzAutoGaming/input/parameters.hpp"

#include <chrono>
#include <cstdint>
#include <gtest/gtest.h>
#include <limits>

namespace Terrahertz::UnitTests {

struct Input_Parameters : public testing::Test
{
    using normal = std::normal_distribution<>;

    double keyDownTimeMean{365.0};
    double keyDownTimeStdDev{122.0};
    double keyUpTimeMean{675.0};
    double keyUpTimeStdDev{178.0};
    double buttonDownTimeMean{221.0};
    double buttonDownTimeStdDev{54.0};
    double buttonUpTimeMean{675.0};
    double buttonUpTimeStdDev{20.0};
    double cursorAccuracyMean{0.0};
    double cursorAccuracyStdDev{0.4};
    double cursorSpeedXMean{222.2};
    double cursorSpeedXStdDev{22.5};
    double cursorSpeedYMean{120.5};
    double cursorSpeedYStdDev{10.6};

    void testCreationWithInvalidValue(double &parameter, double const value) noexcept
    {
        double const temp = parameter;
        parameter         = value;
        EXPECT_FALSE(Input::Parameters::create(normal{keyDownTimeMean, keyDownTimeStdDev},
                                               normal{keyUpTimeMean, keyUpTimeStdDev},
                                               normal{buttonDownTimeMean, buttonDownTimeStdDev},
                                               normal{buttonUpTimeMean, buttonUpTimeStdDev},
                                               normal{cursorAccuracyMean, cursorAccuracyStdDev},
                                               normal{cursorSpeedXMean, cursorSpeedXStdDev},
                                               normal{cursorSpeedYMean, cursorSpeedYStdDev})
                         .has_value());
        parameter = temp;
    }

    void testCreationWithInvalidValues(double &value, double const t) noexcept
    {
        testCreationWithInvalidValue(value, t);
        testCreationWithInvalidValue(value, std::numeric_limits<double>::quiet_NaN());
        testCreationWithInvalidValue(value, std::numeric_limits<double>::infinity());
    }
};

TEST_F(Input_Parameters, CreationReturnsEmptyOptionalIfGivenInvalidParameters)
{
    testCreationWithInvalidValues(keyDownTimeMean, -1.0);
    testCreationWithInvalidValues(keyUpTimeMean, -1.0);
    testCreationWithInvalidValues(buttonDownTimeMean, -1.0);
    testCreationWithInvalidValues(buttonUpTimeMean, -1.0);
    testCreationWithInvalidValues(cursorAccuracyMean, -1.0);
    testCreationWithInvalidValue(cursorAccuracyMean, 1.0);
    testCreationWithInvalidValues(cursorSpeedXMean, -1.0);
    testCreationWithInvalidValues(cursorSpeedYMean, -1.0);
}

TEST_F(Input_Parameters, DataStoredCorrectly)
{
    auto const sut = Input::Parameters::create(normal{keyDownTimeMean, keyDownTimeStdDev},
                                               normal{keyUpTimeMean, keyUpTimeStdDev},
                                               normal{buttonDownTimeMean, buttonDownTimeStdDev},
                                               normal{buttonUpTimeMean, buttonUpTimeStdDev},
                                               normal{cursorAccuracyMean, cursorAccuracyStdDev},
                                               normal{cursorSpeedXMean, cursorSpeedXStdDev},
                                               normal{cursorSpeedYMean, cursorSpeedYStdDev})
                         .value();

    EXPECT_EQ(sut.keyDownTime().mean(), keyDownTimeMean);
    EXPECT_EQ(sut.keyDownTime().stddev(), keyDownTimeStdDev);
    EXPECT_EQ(sut.keyUpTime().mean(), keyUpTimeMean);
    EXPECT_EQ(sut.keyUpTime().stddev(), keyUpTimeStdDev);
    EXPECT_EQ(sut.buttonDownTime().mean(), buttonDownTimeMean);
    EXPECT_EQ(sut.buttonDownTime().stddev(), buttonDownTimeStdDev);
    EXPECT_EQ(sut.buttonUpTime().mean(), buttonUpTimeMean);
    EXPECT_EQ(sut.buttonUpTime().stddev(), buttonUpTimeStdDev);
    EXPECT_EQ(sut.cursorAccuracy().mean(), cursorAccuracyMean);
    EXPECT_EQ(sut.cursorAccuracy().stddev(), cursorAccuracyStdDev);
    EXPECT_EQ(sut.cursorSpeedX().mean(), cursorSpeedXMean);
    EXPECT_EQ(sut.cursorSpeedX().stddev(), cursorSpeedXStdDev);
    EXPECT_EQ(sut.cursorSpeedY().mean(), cursorSpeedYMean);
    EXPECT_EQ(sut.cursorSpeedY().stddev(), cursorSpeedYStdDev);
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
