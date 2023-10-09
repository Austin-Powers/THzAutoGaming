#include "THzAutoGaming/input/emulator.hpp"

#include "THzAutoGaming/input/normalDeviationStrategy.hpp"

#include <chrono>
#include <condition_variable>
#include <cstdint>
#include <gtest/gtest.h>
#include <mutex>
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
        ms   calculateKeyDownTime() noexcept override { return check(1U); }

        void expectCalculateKeyUpTime(ms const returnValue) noexcept { setup(2U, returnValue); }
        ms   calculateKeyUpTime() noexcept override { return check(2U); }

        void expectCalculateButtonDownTime(ms const returnValue) noexcept { setup(3U, returnValue); }
        ms   calculateButtonDownTime() noexcept override { return check(3U); }

        void expectCalculateButtonUpTime(ms const returnValue) noexcept { setup(4U, returnValue); }
        ms   calculateButtonUpTime() noexcept override { return check(4U); }

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
            std::uint32_t returnValue{1000};
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
            double returnValue{1.0};
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
            ex.wheelSteps     = returnValue;
            ex.remainingSteps = input;
            _calls.push_back(ex);
        }
        std::int16_t calculateWheelSteps(std::int16_t const remainingSteps) noexcept override
        {
            std::int16_t returnValue{10};
            if (remainingSteps < 0)
            {
                returnValue = -returnValue;
            }
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
            std::uint16_t returnValue{20};
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
        ms   calculateWheelResetTime() noexcept override { return check(10U); }

    private:
        void setup(std::uint8_t const id, ms const time) noexcept
        {
            Expectation ex;
            ex.id   = id;
            ex.time = time;
            _calls.push_back(ex);
        }
        ms check(std::uint8_t const id) noexcept
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

        ~MockSystemInterface() noexcept { EXPECT_EQ(_calls->size(), 0U) << "Not all expected calls were made"; }

        MockSystemInterface(std::deque<Call> &rCalls, std::condition_variable &emptyCallsSignal) noexcept
            : _calls{&rCalls}, _emptyCallsSignal{&emptyCallsSignal}
        {}

        MockSystemInterface(MockSystemInterface const &other) noexcept
            : _calls{other._calls}, _emptyCallsSignal{other._emptyCallsSignal}
        {}

        MockSystemInterface &operator=(MockSystemInterface const &other) noexcept
        {
            _calls = other._calls;
            return *this;
        }

        void expectIsDown(Input::MouseButton const mb, bool returnValue) noexcept { setup(1U, mb, returnValue); }
        bool isDown(Input::MouseButton const mb) noexcept { return check(1U, mb); }

        void expectIsDown(Input::Key const k, bool returnValue) noexcept { setup(2U, k, returnValue); }
        bool isDown(Input::Key const k) noexcept { return check(2U, k); }

        void expectIsActive(Input::KeyboardLock const l, bool returnValue) noexcept { setup(3U, l, returnValue); }
        bool isActive(Input::KeyboardLock const l) noexcept { return check(3U, l); }

        void expectGetCursorPosition(std::uint32_t const x, std::uint32_t const y, bool returnValue) noexcept
        {
            Call call;
            call.id = 4U;
            call.x  = x;
            call.y  = y;

            call.returnValue = returnValue;
            _calls->push_back(call);
        }
        bool getCursorPosition(std::uint32_t &x, std::uint32_t &y) noexcept
        {
            auto returnValue{true};
            EXPECT_FALSE(_calls->empty()) << "Unexpected additional call";
            if (!_calls->empty())
            {
                EXPECT_EQ(_calls->front().id, 4U) << "Unexpected function call";
                x           = _calls->front().x;
                y           = _calls->front().y;
                returnValue = _calls->front().returnValue;
                popFrontCalls();
            }
            return returnValue;
        }

        void expectSetCursorPosition(std::uint32_t const x, std::uint32_t const y, bool returnValue) noexcept
        {
            Call call;
            call.id = 5U;
            call.x  = x;
            call.y  = y;

            call.returnValue = returnValue;
            _calls->push_back(call);
        }
        bool setCursorPosition(std::uint32_t const x, std::uint32_t const y) noexcept
        {
            auto returnValue{true};
            EXPECT_FALSE(_calls->empty()) << "Unexpected additional call";
            if (!_calls->empty())
            {
                printf("Exp vs Act: X %d vs %d Y %d vs %d\n", _calls->front().x, x, _calls->front().y, y);
                EXPECT_EQ(_calls->front().id, 5U) << "Unexpected function call";
                EXPECT_EQ(_calls->front().x, x) << "Parameter wrong";
                EXPECT_EQ(_calls->front().y, y) << "Parameter wrong";
                returnValue = _calls->front().returnValue;
                popFrontCalls();
            }
            return returnValue;
        }

        void expectTurnMouseWheel(std::int16_t const steps, bool returnValue) noexcept
        {
            setup(6U, steps, returnValue);
        }
        bool turnMouseWheel(std::int16_t const steps) noexcept { return check(6U, steps); }

        void expectDown(Input::MouseButton const mb, bool returnValue) noexcept { setup(7U, mb, returnValue); }
        bool down(Input::MouseButton const mb) noexcept { return check(7U, mb); }

        void expectUp(Input::MouseButton const mb, bool returnValue) noexcept { setup(8U, mb, returnValue); }
        bool up(Input::MouseButton const mb) noexcept { return check(8U, mb); }

        void expectDown(Input::Key const k, bool returnValue) noexcept { setup(9U, k, returnValue); }
        bool down(Input::Key const k) noexcept { return check(9U, k); }

        void expectUp(Input::Key const k, bool returnValue) noexcept { setup(10U, k, returnValue); }
        bool up(Input::Key const k) noexcept { return check(10U, k); }

    private:
        template <typename T>
        void setup(std::uint8_t const id, T const t, bool returnValue) noexcept
        {
            Call call;
            call.id           = id;
            getValue<T>(call) = t;
            call.returnValue  = returnValue;
            _calls->push_back(call);
        }

        template <typename T>
        bool check(std::uint8_t const id, T const t) noexcept
        {
            auto returnValue{true};
            EXPECT_FALSE(_calls->empty()) << "Unexpected additional call";
            if (!_calls->empty())
            {
                EXPECT_EQ(_calls->front().id, id) << "Unexpected function call";
                EXPECT_EQ(getValue<T>(_calls->front()), t);
                returnValue = _calls->front().returnValue;
                popFrontCalls();
            }
            return returnValue;
        }

        template <typename T>
        T &getValue(Call &call) noexcept = delete;

        void popFrontCalls()
        {
            _calls->pop_front();
            if (_calls->empty())
            {
                _emptyCallsSignal->notify_all();
            }
        }

        std::deque<Call> *_calls;

        std::condition_variable *_emptyCallsSignal;
    };

    using TestEmulator = Input::Emulator<MockSystemInterface>;

    static Point toPoint(std::uint32_t const x, std::uint32_t const y) noexcept
    {
        return Point{static_cast<Point::Coordinate>(x), static_cast<Point::Coordinate>(y)};
    }

    void waitForSignal(ms const maxWaitTime = ms{1000U}) noexcept
    {
        std::unique_lock<std::mutex> lock{mutex};
        emptyCallsSignal.wait_for(lock, maxWaitTime);
        // wait for the call to finish
        std::this_thread::sleep_for(ms{1U});
        EXPECT_EQ(sut.actionCountMouse(), 0U);
        EXPECT_EQ(sut.actionCountKeyboard(), 0U);
    }

    std::mutex mutex{};

    std::condition_variable emptyCallsSignal{};

    std::deque<MockSystemInterface::Call> systemInterfaceCalls{};

    MockSystemInterface systemInterface{systemInterfaceCalls, emptyCallsSignal};

    MockStrategy strategy{};

    TestEmulator sut{&strategy, systemInterface};
};

template <>
Input::MouseButton &Input_Emulator::MockSystemInterface::getValue(Call &call) noexcept
{
    return call.mb;
}
template <>
Input::Key &Input_Emulator::MockSystemInterface::getValue(Call &call) noexcept
{
    return call.k;
}
template <>
Input::KeyboardLock &Input_Emulator::MockSystemInterface::getValue(Call &call) noexcept
{
    return call.l;
}
template <>
std::int16_t &Input_Emulator::MockSystemInterface::getValue(Call &call) noexcept
{
    return call.steps;
}

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
    systemInterface.expectGetCursorPosition(23U, 42U, false);
    systemInterface.expectGetCursorPosition(22U, 40U, true);

    std::uint32_t x{};
    std::uint32_t y{};
    EXPECT_FALSE(sut.getCursorPosition(x, y));
    EXPECT_TRUE(sut.getCursorPosition(x, y));
    EXPECT_EQ(x, 22U);
    EXPECT_EQ(y, 40U);
}

TEST_F(Input_Emulator, IsMouseButtonDown)
{
    systemInterface.expectIsDown(Input::MouseButton::Middle, false);
    EXPECT_FALSE(sut.isDown(Input::MouseButton::Middle));
    systemInterface.expectIsDown(Input::MouseButton::Left, true);
    EXPECT_TRUE(sut.isDown(Input::MouseButton::Left));
    EXPECT_EQ(sut.errorCounter(), 0U);
}

TEST_F(Input_Emulator, IsKeyDown)
{
    systemInterface.expectIsDown(Input::Key::Backspace, false);
    EXPECT_FALSE(sut.isDown(Input::Key::Backspace));
    systemInterface.expectIsDown(Input::Key::ControlKey, true);
    EXPECT_TRUE(sut.isDown(Input::Key::ControlKey));
    EXPECT_EQ(sut.errorCounter(), 0U);
}

TEST_F(Input_Emulator, IsKeyboardLockActive)
{
    systemInterface.expectIsActive(Input::KeyboardLock::Caps, false);
    EXPECT_FALSE(sut.isActive(Input::KeyboardLock::Caps));
    systemInterface.expectIsActive(Input::KeyboardLock::Num, true);
    EXPECT_TRUE(sut.isActive(Input::KeyboardLock::Num));
    EXPECT_EQ(sut.errorCounter(), 0U);
}

TEST_F(Input_Emulator, CommandToDoesNotBlockCaller)
{
    strategy.expectCalculateButtonDownTime(ms{2U});
    strategy.expectCalculateButtonUpTime(ms{2U});
    systemInterface.expectDown(Input::MouseButton::Left, true);
    systemInterface.expectUp(Input::MouseButton::Left, true);

    auto const startTime = std::chrono::steady_clock::now();
    sut.click(Input::MouseButton::Left);
    auto const endTime  = std::chrono::steady_clock::now();
    auto const duration = std::chrono::duration_cast<ms>(endTime - startTime);
    EXPECT_LE(duration.count(), 10U);
    waitForSignal();
    EXPECT_EQ(sut.errorCounter(), 0U);
}

TEST_F(Input_Emulator, KeyDown)
{
    strategy.expectCalculateKeyDownTime(ms{2U});
    systemInterface.expectDown(Input::Key::Return, false);
    systemInterface.expectDown(Input::Key::Return, true);
    sut.down(Input::Key::Return);
    waitForSignal();
    EXPECT_EQ(sut.errorCounter(), 1U);
}

TEST_F(Input_Emulator, KeyUp)
{
    strategy.expectCalculateKeyUpTime(ms{2U});
    systemInterface.expectUp(Input::Key::Return, false);
    systemInterface.expectUp(Input::Key::Return, true);
    sut.up(Input::Key::Return);
    waitForSignal();
    EXPECT_EQ(sut.errorCounter(), 1U);
}

TEST_F(Input_Emulator, KeyPress)
{
    strategy.expectCalculateKeyDownTime(ms{2U});
    strategy.expectCalculateKeyUpTime(ms{2U});
    systemInterface.expectDown(Input::Key::Escape, true);
    systemInterface.expectUp(Input::Key::Escape, true);
    sut.press(Input::Key::Escape);
    waitForSignal();
    EXPECT_EQ(sut.errorCounter(), 0U);
}

TEST_F(Input_Emulator, ButtonDown)
{
    strategy.expectCalculateButtonDownTime(ms{2U});
    systemInterface.expectDown(Input::MouseButton::Left, false);
    systemInterface.expectDown(Input::MouseButton::Left, true);
    sut.down(Input::MouseButton::Left);
    waitForSignal();
    EXPECT_EQ(sut.errorCounter(), 1U);
}

TEST_F(Input_Emulator, ButtonUp)
{
    strategy.expectCalculateButtonUpTime(ms{2U});
    systemInterface.expectUp(Input::MouseButton::Middle, false);
    systemInterface.expectUp(Input::MouseButton::Middle, true);
    sut.up(Input::MouseButton::Middle);
    waitForSignal();
    EXPECT_EQ(sut.errorCounter(), 1U);
}

TEST_F(Input_Emulator, Click)
{
    strategy.expectCalculateButtonDownTime(ms{2U});
    strategy.expectCalculateButtonUpTime(ms{2U});
    systemInterface.expectDown(Input::MouseButton::Right, true);
    systemInterface.expectUp(Input::MouseButton::Right, true);
    sut.click(Input::MouseButton::Right);
    waitForSignal();
    EXPECT_EQ(sut.errorCounter(), 0U);
}

TEST_F(Input_Emulator, TurnMouseWheel)
{
    strategy.expectCalculateWheelSpeed(100U);
    strategy.expectCalculateWheelResetTime(ms{100U});
    strategy.expectCalculateWheelSteps(15, 20);

    strategy.expectCalculateWheelSpeed(20U);
    strategy.expectCalculateWheelResetTime(ms{80U});
    strategy.expectCalculateWheelSteps(5, 5);

    strategy.expectCalculateWheelSpeed(100U);
    strategy.expectCalculateWheelResetTime(ms{100U});
    strategy.expectCalculateWheelSteps(-15, -26);

    strategy.expectCalculateWheelSpeed(20U);
    strategy.expectCalculateWheelResetTime(ms{120U});
    strategy.expectCalculateWheelSteps(-11, -11);

    systemInterface.expectTurnMouseWheel(2, false);
    for (auto i = 0U; i < 7U; ++i)
    {
        systemInterface.expectTurnMouseWheel(2, true);
    }
    for (auto i = 0U; i < 6U; ++i)
    {
        systemInterface.expectTurnMouseWheel(1, true);
    }
    for (auto i = 0U; i < 7U; ++i)
    {
        systemInterface.expectTurnMouseWheel(-2, true);
    }
    for (auto i = 0U; i < 12U; ++i)
    {
        systemInterface.expectTurnMouseWheel(-1, true);
    }

    sut.turnMouseWheel(20);
    sut.turnMouseWheel(-26);
    waitForSignal(ms{8000});
    EXPECT_EQ(sut.errorCounter(), 1U);
}

TEST_F(Input_Emulator, MoveToBaseCase)
{
    Rectangle const targetArea{50, 100, 100U, 150U};
    auto const      target = targetArea.center();
    auto const      factor = 1.0;
    auto const      speed  = 100U;

    strategy.expectCalculateTargetIn(target, targetArea);
    strategy.expectCalculateSpeed(speed);
    strategy.expectCalculateHorizontalSpeedFactor(factor);

    Point current{};
    systemInterface.expectGetCursorPosition(current.x, current.y, false);
    for (auto i = 0U; i < 1000U; ++i)
    {
        systemInterface.expectGetCursorPosition(current.x, current.y, true);
        auto const direction = current.direction(target);
        auto const distance  = current.distance(target);
        auto const curSpeed  = (speed / 25) * (1 + ((factor - 1.0) * std::cos(direction)));
        current              = current.angularShift(direction, curSpeed);
        if ((current.x >= target.x) && (current.y >= target.y))
        {
            systemInterface.expectSetCursorPosition(target.x, target.y, true);
            break;
        }
        else
        {
            systemInterface.expectSetCursorPosition(current.x, current.y, true);
        }
    }

    sut.moveTo(targetArea);
    waitForSignal(ms{3000});
    EXPECT_EQ(sut.errorCounter(), 1U);
}

TEST_F(Input_Emulator, MoveToSlow) {}

TEST_F(Input_Emulator, Timing) {}

TEST_F(Input_Emulator, ActionCountMouse) {}

TEST_F(Input_Emulator, ActionCountKeyboard) {}

TEST_F(Input_Emulator, Wait) {}

TEST_F(Input_Emulator, Clear) {}

TEST_F(Input_Emulator, Reset) {}

TEST_F(Input_Emulator, Sync) {}

TEST_F(Input_Emulator, MoveToClick) {}

TEST_F(Input_Emulator, DragAndDrop) {}

} // namespace Terrahertz::UnitTests
