#include "THzAutoGaming/input/emulator.hpp"

#include <cstdint>
#include <gtest/gtest.h>

namespace Terrahertz::UnitTests {

struct Input_Emulator : public testing::Test
{
    struct MockSystemInterface
    {};

    using TestEmulator = Input::Emulator<MockSystemInterface>;

    void checkParameters(Input::Parameters const &expected, Input::Parameters const &actual) noexcept
    {
        EXPECT_EQ(expected.keyDownTime().mean, actual.keyDownTime().mean);
        EXPECT_EQ(expected.keyDownTime().stddev, actual.keyDownTime().stddev);
        EXPECT_EQ(expected.keyUpTime().mean, actual.keyUpTime().mean);
        EXPECT_EQ(expected.keyUpTime().stddev, actual.keyUpTime().stddev);
        EXPECT_EQ(expected.buttonDownTime().mean, actual.buttonDownTime().mean);
        EXPECT_EQ(expected.buttonDownTime().stddev, actual.buttonDownTime().stddev);
        EXPECT_EQ(expected.buttonUpTime().mean, actual.buttonUpTime().mean);
        EXPECT_EQ(expected.buttonUpTime().stddev, actual.buttonUpTime().stddev);
        EXPECT_EQ(expected.cursorAccuracy(), actual.cursorAccuracy());
        EXPECT_EQ(expected.cursorSpeedX().mean, actual.cursorSpeedX().mean);
        EXPECT_EQ(expected.cursorSpeedX().stddev, actual.cursorSpeedX().stddev);
        EXPECT_EQ(expected.cursorSpeedY().mean, actual.cursorSpeedY().mean);
        EXPECT_EQ(expected.cursorSpeedY().stddev, actual.cursorSpeedY().stddev);
    }
};

TEST_F(Input_Emulator, ParametersCorrectAfterConstruction)
{
    auto const         params = Input::Parameters::Human();
    TestEmulator const sut{params};
    checkParameters(params, sut.parametes());
}

} // namespace Terrahertz::UnitTests
