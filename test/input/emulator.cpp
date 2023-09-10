#include "THzAutoGaming/input/emulator.hpp"

#include "THzAutoGaming/input/normalDeviationStrategy.hpp"

#include <chrono>
#include <cstdint>
#include <gtest/gtest.h>
#include <thread>

namespace Terrahertz::UnitTests {

struct Input_Emulator : public testing::Test
{
    using ms = std::chrono::milliseconds;

    struct MockSystemInterface
    {
        struct Data
        {
            bool returnValue{};

            bool isDownCalled{};

            bool downCalled{};

            bool upCalled{};

            Input::MouseButton mb{};

            Input::Key k{};

            Input::KeyboardLock l{};

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

        bool isDown(Input::MouseButton const mb) noexcept
        {
            data->mb           = mb;
            data->isDownCalled = true;
            return data->returnValue;
        }

        bool isDown(Input::Key const k) noexcept
        {
            data->k            = k;
            data->isDownCalled = true;
            return data->returnValue;
        }

        bool isActive(Input::KeyboardLock const l) noexcept
        {
            data->l = l;
            return data->returnValue;
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

        bool down(Input::MouseButton const mb) noexcept
        {
            data->mb = mb;
            return data->returnValue;
        }

        bool up(Input::MouseButton const mb) noexcept
        {
            data->mb = mb;
            return data->returnValue;
        }

        bool down(Input::Key const k) noexcept
        {
            data->k = k;
            return data->returnValue;
        }

        bool up(Input::Key const k) noexcept
        {
            data->k = k;
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
        EXPECT_EQ(expected.keyDownTime().mean(), actual.keyDownTime().mean());
        EXPECT_EQ(expected.keyDownTime().stddev(), actual.keyDownTime().stddev());
        EXPECT_EQ(expected.keyUpTime().mean(), actual.keyUpTime().mean());
        EXPECT_EQ(expected.keyUpTime().stddev(), actual.keyUpTime().stddev());
        EXPECT_EQ(expected.buttonDownTime().mean(), actual.buttonDownTime().mean());
        EXPECT_EQ(expected.buttonDownTime().stddev(), actual.buttonDownTime().stddev());
        EXPECT_EQ(expected.buttonUpTime().mean(), actual.buttonUpTime().mean());
        EXPECT_EQ(expected.buttonUpTime().stddev(), actual.buttonUpTime().stddev());
        EXPECT_EQ(expected.cursorAccuracy(), actual.cursorAccuracy());
        EXPECT_EQ(expected.cursorSpeedX().mean(), actual.cursorSpeedX().mean());
        EXPECT_EQ(expected.cursorSpeedX().stddev(), actual.cursorSpeedX().stddev());
        EXPECT_EQ(expected.cursorSpeedY().mean(), actual.cursorSpeedY().mean());
        EXPECT_EQ(expected.cursorSpeedY().stddev(), actual.cursorSpeedY().stddev());
    }
};

TEST_F(Input_Emulator, ConstructionCorrect)
{
    EXPECT_EQ(sut.errorCounter(), 0U);
    EXPECT_EQ(sut.actionCountMouse(), 0U);
    EXPECT_EQ(sut.actionCountKeyboard(), 0U);
}

TEST_F(Input_Emulator, ParametersCorrectAfterConstruction)
{
    auto const strategy = dynamic_cast<Input::NormalDeviationStrategy *>(sut.strategy());
    checkParameters(Input::Parameters::Human(), strategy->parameters());
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

TEST_F(Input_Emulator, IsKeyDown)
{
    EXPECT_FALSE(sut.isDown(Input::Key::Backspace));
    EXPECT_EQ(systemInterfaceData.k, Input::Key::Backspace);
    systemInterfaceData.returnValue = true;
    EXPECT_TRUE(sut.isDown(Input::Key::ControlKey));
    EXPECT_EQ(systemInterfaceData.k, Input::Key::ControlKey);
}

TEST_F(Input_Emulator, IsKeyboardLockActive)
{
    EXPECT_FALSE(sut.isActive(Input::KeyboardLock::Caps));
    EXPECT_EQ(systemInterfaceData.l, Input::KeyboardLock::Caps);
    systemInterfaceData.returnValue = true;
    EXPECT_TRUE(sut.isActive(Input::KeyboardLock::Num));
    EXPECT_EQ(systemInterfaceData.l, Input::KeyboardLock::Num);
}

TEST_F(Input_Emulator, MoveToDoesNotBlockCaller)
{
    auto const      startTime = std::chrono::steady_clock::now();
    Rectangle const target{100, 100, 100U, 100U};
    sut.moveTo(target);
    auto const endTime  = std::chrono::steady_clock::now();
    auto const duration = std::chrono::duration_cast<ms>(endTime - startTime);
    EXPECT_LE(duration.count(), 10U);
}

TEST_F(Input_Emulator, MoveToResultsInCallOfSetCursorPosition)
{
    // systemInterfaceData.returnValue = true;
    // sut.moveTo(100U, 100U);
    // std::this_thread::sleep_for(ms{10});
    // EXPECT_EQ(systemInterfaceData.x, 100U);
    // EXPECT_EQ(systemInterfaceData.y, 100U);
}

} // namespace Terrahertz::UnitTests
