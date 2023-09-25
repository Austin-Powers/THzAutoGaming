#include "THzAutoGaming/input/emulator.hpp"

#include "THzAutoGaming/input/normalDeviationStrategy.hpp"

#include <chrono>
#include <cstdint>
#include <gtest/gtest.h>
#include <thread>
#include <vector>

namespace Terrahertz::UnitTests {

struct Input_Emulator : public testing::Test
{
    using ms = std::chrono::milliseconds;

    struct MockStrategy : public Input::IDeviationStrategy
    {
        ~MockStrategy() noexcept { EXPECT_TRUE(_calls.empty()) << "Not all expected calls were made"; }

        void expectCalculateKeyDownTime(ms const returnValue) noexcept { setup(1U, returnValue); }
        ms   calculateKeyDownTime() noexcept override { return call(1U); }

        void expectCalculateKeyUpTime(ms const returnValue) noexcept { setup(2U, returnValue); }
        ms   calculateKeyUpTime() noexcept override { return call(2U); }

        void expectCalculateButtonDownTime(ms const returnValue) noexcept { setup(3U, returnValue); }
        ms   calculateButtonDownTime() noexcept override { return call(3U); }

        void expectCalculateButtonUpTime(ms const returnValue) noexcept { setup(4U, returnValue); }
        ms   calculateButtonUpTime() noexcept override { return call(4U); }

        void expectCalculateTargetIn(Point const returnValue, Rectangle const &input) noexcept
        {
            Expectation ex;
            ex.id     = 5U;
            ex.target = returnValue;
            ex.area   = input;
            _calls.push_back(ex);
        }
        Point calculateTargetIn(Rectangle const &area) noexcept override
        {
            Point returnValue{};
            EXPECT_FALSE(_calls.empty()) << "Unexpected additional call";
            if (!_calls.empty())
            {
                EXPECT_EQ(_calls.front().id, 5U) << "Unexpected function call";
                EXPECT_EQ(_calls.front().area, area);
                returnValue = _calls.front().target;
                _calls.pop_front();
            }
            return returnValue;
        }

        void expectCalculateSpeed(std::uint32_t const returnValue) noexcept
        {
            Expectation ex;
            ex.id    = 6U;
            ex.speed = returnValue;
            _calls.push_back(ex);
        }
        std::uint32_t calculateSpeed() noexcept override
        {
            std::uint32_t returnValue{};
            EXPECT_FALSE(_calls.empty()) << "Unexpected additional call";
            if (!_calls.empty())
            {
                EXPECT_EQ(_calls.front().id, 6U) << "Unexpected function call";
                returnValue = _calls.front().speed;
                _calls.pop_front();
            }
            return returnValue;
        }

        void expectCalculateHorizontalSpeedFactor(double const returnValue) noexcept
        {
            Expectation ex;
            ex.id     = 7U;
            ex.factor = returnValue;
            _calls.push_back(ex);
        }
        double calculateHorizontalSpeedFactor() noexcept override
        {
            double returnValue{};
            EXPECT_FALSE(_calls.empty()) << "Unexpected additional call";
            if (!_calls.empty())
            {
                EXPECT_EQ(_calls.front().id, 7U) << "Unexpected function call";
                returnValue = _calls.front().factor;
                _calls.pop_front();
            }
            return returnValue;
        }

        void expectCalculateWheelSteps(std::int16_t const returnValue, std::int16_t const input) noexcept
        {
            Expectation ex;
            ex.id             = 8U;
            ex.factor         = returnValue;
            ex.remainingSteps = input;
            _calls.push_back(ex);
        }
        std::int16_t calculateWheelSteps(std::int16_t const remainingSteps) noexcept override
        {
            std::int16_t returnValue{};
            EXPECT_FALSE(_calls.empty()) << "Unexpected additional call";
            if (!_calls.empty())
            {
                EXPECT_EQ(_calls.front().id, 8U) << "Unexpected function call";
                EXPECT_EQ(_calls.front().remainingSteps, remainingSteps);
                returnValue = _calls.front().wheelSteps;
                _calls.pop_front();
            }
            return returnValue;
        }

        void expectCalculateWheelSpeed(std::uint16_t const returnValue) noexcept
        {
            Expectation ex;
            ex.id         = 9U;
            ex.wheelSpeed = returnValue;
            _calls.push_back(ex);
        }
        std::uint16_t calculateWheelSpeed() noexcept override
        {
            std::uint16_t returnValue{};
            EXPECT_FALSE(_calls.empty()) << "Unexpected additional call";
            if (!_calls.empty())
            {
                EXPECT_EQ(_calls.front().id, 9U) << "Unexpected function call";
                returnValue = _calls.front().wheelSpeed;
                _calls.pop_front();
            }
            return returnValue;
        }

        void expectCalculateWheelResetTime(ms const returnValue) noexcept { setup(10U, returnValue); }
        ms   calculateWheelResetTime() noexcept override { return call(10U); }

    private:
        void setup(std::uint8_t const id, ms const time) noexcept
        {
            Expectation ex;
            ex.id   = 1U;
            ex.time = time;
            _calls.push_back(ex);
        }
        ms call(std::uint8_t const id) noexcept
        {
            ms returnValue{};
            EXPECT_FALSE(_calls.empty()) << "Unexpected additional call";
            if (!_calls.empty())
            {
                EXPECT_EQ(_calls.front().id, id) << "Unexpected function call";
                returnValue = _calls.front().time;
                _calls.pop_front();
            }
            return returnValue;
        }

        struct Expectation
        {
            std::uint8_t id;
            std::int16_t remainingSteps;
            Point        target;
            Rectangle    area;
            union
            {
                std::int16_t  wheelSteps;
                std::uint16_t wheelSpeed;
                std::uint32_t speed;
                double        factor;
                ms            time;
            };
        };
        std::deque<Expectation> _calls{};
    };

    struct MockSystemInterface
    {
        struct Call
        {
            std::uint8_t id;

            bool returnValue{};

            union
            {
                Input::MouseButton  mb;
                Input::Key          k;
                Input::KeyboardLock l;
                std::int16_t        steps;
                struct
                {
                    std::uint32_t x;
                    std::uint32_t y;
                };
            };
        };

        MockSystemInterface(std::deque<Call> &rCalls) noexcept : calls{&rCalls} {}

        MockSystemInterface(MockSystemInterface const &other) noexcept : calls{other.calls} {}

        MockSystemInterface &operator=(MockSystemInterface const &other) noexcept
        {
            calls = other.calls;
            return *this;
        }

        bool isDown(Input::MouseButton const mb) noexcept
        {
            auto returnValue{false};
            EXPECT_FALSE(calls->empty()) << "Unexpected additional call";
            if (!calls->empty())
            {
                EXPECT_EQ(calls->front().id, 1U) << "Unexpected function call";
                EXPECT_EQ(calls->front().mb, mb) << "Parameter wrong";
                returnValue = calls->front().returnValue;
                calls->pop_front();
            }
            return returnValue;
        }

        bool isDown(Input::Key const k) noexcept
        {
            auto returnValue{false};
            EXPECT_FALSE(calls->empty()) << "Unexpected additional call";
            if (!calls->empty())
            {
                EXPECT_EQ(calls->front().id, 2U) << "Unexpected function call";
                EXPECT_EQ(calls->front().k, k) << "Parameter wrong";
                returnValue = calls->front().returnValue;
                calls->pop_front();
            }
            return returnValue;
        }

        bool isActive(Input::KeyboardLock const l) noexcept
        {
            auto returnValue{false};
            EXPECT_FALSE(calls->empty()) << "Unexpected additional call";
            if (!calls->empty())
            {
                EXPECT_EQ(calls->front().id, 3U) << "Unexpected function call";
                EXPECT_EQ(calls->front().l, l) << "Parameter wrong";
                returnValue = calls->front().returnValue;
                calls->pop_front();
            }
            return returnValue;
        }

        bool getCursorPosition(std::uint32_t &x, std::uint32_t &y) noexcept
        {
            auto returnValue{false};
            EXPECT_FALSE(calls->empty()) << "Unexpected additional call";
            if (!calls->empty())
            {
                EXPECT_EQ(calls->front().id, 4U) << "Unexpected function call";
                x           = calls->front().x;
                y           = calls->front().y;
                returnValue = calls->front().returnValue;
                calls->pop_front();
            }
            return returnValue;
        }

        bool setCursorPosition(std::uint32_t const x, std::uint32_t const y) noexcept
        {
            auto returnValue{false};
            EXPECT_FALSE(calls->empty()) << "Unexpected additional call";
            if (!calls->empty())
            {
                EXPECT_EQ(calls->front().id, 5U) << "Unexpected function call";
                EXPECT_EQ(calls->front().x, x) << "Parameter wrong";
                EXPECT_EQ(calls->front().y, y) << "Parameter wrong";
                returnValue = calls->front().returnValue;
                calls->pop_front();
            }
            return returnValue;
        }

        bool turnMouseWheel(std::int16_t const steps) noexcept
        {
            auto returnValue{false};
            EXPECT_FALSE(calls->empty()) << "Unexpected additional call";
            if (!calls->empty())
            {
                EXPECT_EQ(calls->front().id, 6U) << "Unexpected function call";
                EXPECT_EQ(calls->front().steps, steps) << "Parameter wrong";
                returnValue = calls->front().returnValue;
                calls->pop_front();
            }
            return returnValue;
        }

        bool down(Input::MouseButton const mb) noexcept
        {
            auto returnValue{false};
            EXPECT_FALSE(calls->empty()) << "Unexpected additional call";
            if (!calls->empty())
            {
                EXPECT_EQ(calls->front().id, 7U) << "Unexpected function call";
                EXPECT_EQ(calls->front().mb, mb) << "Parameter wrong";
                returnValue = calls->front().returnValue;
                calls->pop_front();
            }
            return returnValue;
        }

        bool up(Input::MouseButton const mb) noexcept
        {
            auto returnValue{false};
            EXPECT_FALSE(calls->empty()) << "Unexpected additional call";
            if (!calls->empty())
            {
                EXPECT_EQ(calls->front().id, 8U) << "Unexpected function call";
                EXPECT_EQ(calls->front().mb, mb) << "Parameter wrong";
                returnValue = calls->front().returnValue;
                calls->pop_front();
            }
            return returnValue;
        }

        bool down(Input::Key const k) noexcept
        {
            auto returnValue{false};
            EXPECT_FALSE(calls->empty()) << "Unexpected additional call";
            if (!calls->empty())
            {
                EXPECT_EQ(calls->front().id, 9U) << "Unexpected function call";
                EXPECT_EQ(calls->front().k, k) << "Parameter wrong";
                returnValue = calls->front().returnValue;
                calls->pop_front();
            }
            return returnValue;
        }

        bool up(Input::Key const k) noexcept
        {
            auto returnValue{false};
            EXPECT_FALSE(calls->empty()) << "Unexpected additional call";
            if (!calls->empty())
            {
                EXPECT_EQ(calls->front().id, 10U) << "Unexpected function call";
                EXPECT_EQ(calls->front().k, k) << "Parameter wrong";
                returnValue = calls->front().returnValue;
                calls->pop_front();
            }
            return returnValue;
        }

        std::deque<Call> *calls;
    };

    using TestEmulator = Input::Emulator<MockSystemInterface>;

    std::deque<MockSystemInterface::Call> systemInterfaceCalls{};

    MockSystemInterface systemInterface{systemInterfaceCalls};

    MockStrategy strategy{};

    TestEmulator sut{&strategy, systemInterface};
};

TEST_F(Input_Emulator, ConstructionCorrect)
{
    EXPECT_EQ(sut.errorCounter(), 0U);
    EXPECT_EQ(sut.actionCountMouse(), 0U);
    EXPECT_EQ(sut.actionCountKeyboard(), 0U);
    EXPECT_EQ(sut.strategy(), &strategy);
}

TEST_F(Input_Emulator, ParametersCorrectAfterConstruction)
{
    TestEmulator emulator{Input::Parameters::Human(), systemInterface};
    auto const   strategy = dynamic_cast<Input::NormalDeviationStrategy *>(emulator.strategy());
    auto const   expected = Input::Parameters::Human();
    auto const   actual   = strategy->parameters();
    EXPECT_EQ(expected.keyDownTime(), actual.keyDownTime());
    EXPECT_EQ(expected.keyUpTime(), actual.keyUpTime());
    EXPECT_EQ(expected.buttonDownTime(), actual.buttonDownTime());
    EXPECT_EQ(expected.buttonUpTime(), actual.buttonUpTime());
    EXPECT_EQ(expected.cursorAccuracy(), actual.cursorAccuracy());
    EXPECT_EQ(expected.cursorSpeed(), actual.cursorSpeed());
    EXPECT_EQ(expected.horizontalSpeedFactor(), actual.horizontalSpeedFactor());
    EXPECT_EQ(expected.wheelStepsPerPush(), actual.wheelStepsPerPush());
    EXPECT_EQ(expected.wheelSpeed(), actual.wheelSpeed());
    EXPECT_EQ(expected.wheelResetTime(), actual.wheelResetTime());
}

TEST_F(Input_Emulator, GetCursorPositionRelaisInformationCorrectly)
{
    // std::uint32_t x{};
    // std::uint32_t y{};
    // EXPECT_FALSE(sut.getCursorPosition(x, y));
    // systemInterfaceData.returnValue = true;
    // systemInterfaceData.x           = 22U;
    // systemInterfaceData.y           = 40U;
    // EXPECT_TRUE(sut.getCursorPosition(x, y));
    // EXPECT_EQ(x, systemInterfaceData.x);
    // EXPECT_EQ(y, systemInterfaceData.y);
}

TEST_F(Input_Emulator, IsMouseButtonDown)
{
    // EXPECT_FALSE(sut.isDown(Input::MouseButton::Middle));
    // EXPECT_EQ(systemInterfaceData.mb, Input::MouseButton::Middle);
    // systemInterfaceData.returnValue = true;
    // EXPECT_TRUE(sut.isDown(Input::MouseButton::Left));
    // EXPECT_EQ(systemInterfaceData.mb, Input::MouseButton::Left);
}

TEST_F(Input_Emulator, IsKeyDown)
{
    // EXPECT_FALSE(sut.isDown(Input::Key::Backspace));
    // EXPECT_EQ(systemInterfaceData.k, Input::Key::Backspace);
    // systemInterfaceData.returnValue = true;
    // EXPECT_TRUE(sut.isDown(Input::Key::ControlKey));
    // EXPECT_EQ(systemInterfaceData.k, Input::Key::ControlKey);
}

TEST_F(Input_Emulator, IsKeyboardLockActive)
{
    // EXPECT_FALSE(sut.isActive(Input::KeyboardLock::Caps));
    // EXPECT_EQ(systemInterfaceData.l, Input::KeyboardLock::Caps);
    // systemInterfaceData.returnValue = true;
    // EXPECT_TRUE(sut.isActive(Input::KeyboardLock::Num));
    // EXPECT_EQ(systemInterfaceData.l, Input::KeyboardLock::Num);
}

TEST_F(Input_Emulator, CommandToDoesNotBlockCaller)
{
    // auto const startTime = std::chrono::steady_clock::now();
    // sut.click(Input::MouseButton::Left);
    // auto const endTime  = std::chrono::steady_clock::now();
    // auto const duration = std::chrono::duration_cast<ms>(endTime - startTime);
    // EXPECT_LE(duration.count(), 10U);
}

TEST_F(Input_Emulator, KeyDown)
{
    sut.down(Input::Key::Return);
    // interface is called, returns error
    // check error counter
    // check if emulator tries again, return no error
}

TEST_F(Input_Emulator, ActionCountMouse) {}

TEST_F(Input_Emulator, ActionCountKeyboard) {}

TEST_F(Input_Emulator, Wait) {}

TEST_F(Input_Emulator, Clear) {}

TEST_F(Input_Emulator, Reset) {}

TEST_F(Input_Emulator, Sync) {}

TEST_F(Input_Emulator, MoveTo) {}

TEST_F(Input_Emulator, Click) {}

TEST_F(Input_Emulator, MoveToClick) {}

TEST_F(Input_Emulator, DragAndDrop) {}

TEST_F(Input_Emulator, ButtonDown) {}

TEST_F(Input_Emulator, ButtonUp) {}

TEST_F(Input_Emulator, TurnMouseWheel) {}

TEST_F(Input_Emulator, KeyUp) {}

TEST_F(Input_Emulator, KeyPress) {}

TEST_F(Input_Emulator, Timing) {}

} // namespace Terrahertz::UnitTests
