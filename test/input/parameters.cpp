#include "THzAutoGaming/input/parameters.hpp"

#include <chrono>
#include <cstdint>
#include <gtest/gtest.h>
#include <limits>

namespace Terrahertz::UnitTests {

struct Input_Parameters : public testing::Test
{
    using normal = std::normal_distribution<>;

    double        keyDownTimeMean{365.0};
    double        keyDownTimeStdDev{122.0};
    double        keyUpTimeMean{675.0};
    double        keyUpTimeStdDev{178.0};
    double        buttonDownTimeMean{221.0};
    double        buttonDownTimeStdDev{54.0};
    double        buttonUpTimeMean{675.0};
    double        buttonUpTimeStdDev{20.0};
    double        cursorAccuracyMean{0.0};
    double        cursorAccuracyStdDev{0.4};
    double        cursorSpeedMean{120.5};
    double        cursorSpeedStdDev{10.6};
    double        horizontalSpeedFactor{1.23};
    std::uint16_t wheelStepsPerPush{46U};
    std::uint16_t wheelSpeed{117U};
    double        wheelResetTimeMean{128.16};
    double        wheelResetTimeStdDev{28.16};

    template <typename T>
    void testCreationWithInvalidValue(T &parameter, T const value) noexcept
    {
        T const temp = parameter;
        parameter    = value;
        EXPECT_FALSE(Input::Parameters::create(normal{keyDownTimeMean, keyDownTimeStdDev},
                                               normal{keyUpTimeMean, keyUpTimeStdDev},
                                               normal{buttonDownTimeMean, buttonDownTimeStdDev},
                                               normal{buttonUpTimeMean, buttonUpTimeStdDev},
                                               normal{cursorAccuracyMean, cursorAccuracyStdDev},
                                               normal{cursorSpeedMean, cursorSpeedStdDev},
                                               horizontalSpeedFactor,
                                               wheelStepsPerPush,
                                               wheelSpeed,
                                               normal{wheelResetTimeMean, wheelResetTimeStdDev})
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
    testCreationWithInvalidValues(cursorSpeedMean, 0.0);
    testCreationWithInvalidValues(cursorSpeedMean, -1.0);
    testCreationWithInvalidValues(horizontalSpeedFactor, 0.0);
    testCreationWithInvalidValues(horizontalSpeedFactor, -1.0);
    testCreationWithInvalidValues(wheelResetTimeMean, -1.0);
    testCreationWithInvalidValue<std::uint16_t>(wheelStepsPerPush, 0U);
    testCreationWithInvalidValue<std::uint16_t>(wheelSpeed, 0U);
}

TEST_F(Input_Parameters, DataStoredCorrectly)
{
    auto const sut = Input::Parameters::create(normal{keyDownTimeMean, keyDownTimeStdDev},
                                               normal{keyUpTimeMean, keyUpTimeStdDev},
                                               normal{buttonDownTimeMean, buttonDownTimeStdDev},
                                               normal{buttonUpTimeMean, buttonUpTimeStdDev},
                                               normal{cursorAccuracyMean, cursorAccuracyStdDev},
                                               normal{cursorSpeedMean, cursorSpeedStdDev},
                                               horizontalSpeedFactor,
                                               wheelStepsPerPush,
                                               wheelSpeed,
                                               normal{wheelResetTimeMean, wheelResetTimeStdDev})
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
    EXPECT_EQ(sut.cursorSpeed().mean(), cursorSpeedMean);
    EXPECT_EQ(sut.cursorSpeed().stddev(), cursorSpeedStdDev);
    EXPECT_EQ(sut.horizontalSpeedFactor(), horizontalSpeedFactor);
    EXPECT_EQ(sut.wheelStepsPerPush(), wheelStepsPerPush);
    EXPECT_EQ(sut.wheelSpeed(), wheelSpeed);
    EXPECT_EQ(sut.wheelResetTime().mean(), wheelResetTimeMean);
    EXPECT_EQ(sut.wheelResetTime().stddev(), wheelResetTimeStdDev);
}

TEST_F(Input_Parameters, PresetsValid)
{
    auto const checkParameters = [](Input::Parameters const &parameters) noexcept {
        EXPECT_TRUE(Input::Parameters::create(parameters.keyDownTime(),
                                              parameters.keyUpTime(),
                                              parameters.buttonDownTime(),
                                              parameters.buttonUpTime(),
                                              parameters.cursorAccuracy(),
                                              parameters.cursorSpeed(),
                                              parameters.horizontalSpeedFactor(),
                                              parameters.wheelStepsPerPush(),
                                              parameters.wheelSpeed(),
                                              parameters.wheelResetTime())
                        .has_value());
    };
    checkParameters(Input::Parameters::Human());
    checkParameters(Input::Parameters::Fast());
}

} // namespace Terrahertz::UnitTests
