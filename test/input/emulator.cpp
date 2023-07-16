#include "THzAutoGaming/input/emulator.hpp"

#include <cstdint>
#include <gtest/gtest.h>

namespace Terrahertz::UnitTests {

struct Input_Emulator : public testing::Test
{
    struct MockSystemInterface
    {
        struct Data
        {
            bool returnValue{};

            Input::MouseButton mb{};

            std::uint32_t x{};

            std::uint32_t y{};
        };

        MockSystemInterface(Data &rData) noexcept : data{&rData} {}

        MockSystemInterface(MockSystemInterface const &other) noexcept : data{other.data} {}

        MockSystemInterface &operator=(MockSystemInterface const &other) noexcept
        {
            data = other.data;
            return *this;
        }

        bool getCursorPosition(std::uint32_t &x, std::uint32_t &y) noexcept
        {
            x = data->x;
            y = data->y;
            return data->returnValue;
        }

        bool setCursorPosition(std::uint32_t const x, std::uint32_t const y) noexcept
        {
            data->x = x;
            data->y = y;
            return data->returnValue;
        }

        bool isDown(Input::MouseButton const mb) noexcept
        {
            data->mb = mb;
            return data->returnValue;
        }

        Data *data;
    };

    using TestEmulator = Input::Emulator<MockSystemInterface>;

    MockSystemInterface::Data systemInterfaceData{};

    MockSystemInterface systemInterface{systemInterfaceData};

    TestEmulator sut{Input::Parameters::Human(), systemInterface};

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
    checkParameters(Input::Parameters::Human(), sut.parametes());
}

TEST_F(Input_Emulator, getCursorPositionRelaisInformationCorrectly)
{
    std::uint32_t x{};
    std::uint32_t y{};
    EXPECT_FALSE(sut.getCursorPosition(x, y));
    systemInterfaceData.returnValue = true;
    systemInterfaceData.x           = 22U;
    systemInterfaceData.y           = 40U;
    EXPECT_TRUE(sut.getCursorPosition(x, y));
    EXPECT_EQ(x, systemInterfaceData.x);
    EXPECT_EQ(y, systemInterfaceData.y);
}

TEST_F(Input_Emulator, IsMouseButtonDown)
{
    EXPECT_FALSE(sut.isDown(Input::MouseButton::Middle));
    EXPECT_EQ(systemInterfaceData.mb, Input::MouseButton::Middle);
    systemInterfaceData.returnValue = true;
    EXPECT_TRUE(sut.isDown(Input::MouseButton::Left));
    EXPECT_EQ(systemInterfaceData.mb, Input::MouseButton::Left);
}

} // namespace Terrahertz::UnitTests
