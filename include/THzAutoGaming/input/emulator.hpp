#ifndef THZ_AUTO_GAMING_INPUT_EMULATOR_HPP
#define THZ_AUTO_GAMING_INPUT_EMULATOR_HPP

#include "THzCommon/math/rectangle.hpp"
#include "THzCommon/utility/workerThread.hpp"
#include "common.hpp"
#include "iDeviationStrategy.hpp"
#include "normalDeviationStrategy.hpp"
#include "parameters.hpp"

#include <algorithm>
#include <deque>
#include <string_view>
#include <utility>

namespace Terrahertz::Input {

/// @brief Emulates user input via keyboard and mouse.
template <SystemInterface TSystemInterface>
class Emulator
{
public:
    /// @brief Initializes a new Emulator using the given parameters and interface.
    ///
    /// @param parameters The parameter used for the NormalDeviationStrategy.
    /// @param sysInterface The interface to use for the inputs.
    Emulator(Parameters const &pParameters, TSystemInterface pInterface = {}) noexcept
        : _defaultStrategy{pParameters}, _strategy{&_defaultStrategy}, _interface{pInterface}
    {
        _worker.thread = std::thread([this]() { threadMethod(); });
    }

    /// @brief Initializes a new Emulator using the given deviation strategy.
    ///
    /// @param strategy Pointer to the strategy that should be used by the emulator.
    /// @param pInterface The interface to use for the inputs.
    Emulator(IDeviationStrategy *const strategy, TSystemInterface pInterface = {}) noexcept
        : _defaultStrategy{Parameters::Fast()}, _strategy{strategy}, _interface{pInterface}
    {
        _worker.thread = std::thread([this]() { threadMethod(); });
    }

    /// @brief Finalizes this Emulator instance.
    ~Emulator() noexcept { _worker.shutdown(); }

    /// @brief Returns a pointer to the strategy used by the emulator.
    ///
    /// @return A pointer to the strategy used by the emulator.
    [[nodiscard]] inline IDeviationStrategy *strategy() const noexcept { return _strategy; }

    /// @brief Returns the current position of the cursor on the screen.
    ///
    /// @param x The x coordinate of the cursor.
    /// @param y The y coordinate of the cursor.
    /// @return True if the operation was successfull, false otherwise.
    [[nodiscard]] bool getCursorPosition(std::uint32_t &x, std::uint32_t &y) noexcept
    {
        return _interface.getCursorPosition(x, y);
    }

    /// @brief Returns the state of the given mouse button.
    ///
    /// @param button The button to check.
    /// @return True if the button is down, false otherwise.
    [[nodiscard]] bool isDown(MouseButton const button) noexcept { return _interface.isDown(button); }

    /// @brief Returns the state of the given key.
    ///
    /// @param key The key to check.
    /// @return True if the key is down, false otherwise.
    [[nodiscard]] bool isDown(Key const key) noexcept { return _interface.isDown(key); }

    /// @brief Returns the state of the given lock.
    ///
    /// @param lock The lock to check.
    /// @return True if the lock is active, false otherwise.
    [[nodiscard]] bool isActive(KeyboardLock const lock) noexcept { return _interface.isActive(lock); }

    /// @brief Returns the number of errors encountered during the run of this emulator.
    ///
    /// @return The error counter.
    [[nodiscard]] std::uint32_t errorCounter() const noexcept { return _errorCounter; }

    /// @brief Returns the number of actions that are still scheduled for the mouse.
    ///
    /// @return The number of actions that are still scheduled for the mouse.
    [[nodiscard]] size_t actionCountMouse() const noexcept { return _mouseActions.size(); }

    /// @brief Returns the number of actions that are still scheduled for the keyboard.
    ///
    /// @return The numbre of actions that are still scheduled for the keyboard.
    [[nodiscard]] size_t actionCountKeyboard() const noexcept { return _keyboardActions.size(); }

    /// @brief Adds a wait action to the either the mouse or the keyboard queue.
    ///
    /// @param mouse True if the wait shall be put in the mouse queue, false for the keyborad queue.
    /// @param duration The duration to wait for.
    void wait(bool const mouse, std::chrono::milliseconds const duration) noexcept
    {
        if (mouse)
        {
            addMouseAction(MouseAction::Type::None, duration);
        }
        else
        {
            addKeyboardAction(KeyboardAction::Type::None, duration);
        }
    }

    /// @brief Clears the mouse and keyborad queue.
    void clear() noexcept
    {
        WorkerThread::UniqueLock lock{_worker.mutex};
        _mouseActions.clear();
        _keyboardActions.clear();
    }

    /// @brief Resets all buttons, keys and locks to be up/inactive.
    ///
    /// @remarks This will be done via actions, so calling clear directly after this will cancel the reset.
    void reset() noexcept
    {
        if (isActive(KeyboardLock::Scroll))
        {
            press(Key::ScrollLock);
        }
        if (isActive(KeyboardLock::Num))
        {
            press(Key::NumLock);
        }
        if (isActive(KeyboardLock::Caps))
        {
            press(Key::CapsLock);
        }

        for (auto const button : MouseButtons)
        {
            if (isDown(button))
            {
                up(button);
            }
        }

        for (auto const key : Keys)
        {
            if (isDown(key))
            {
                up(key);
            }
        }
    }

    /// @brief Inserts a wait command into either mouse or keyboard queue to wait until the other queue is finished.
    void sync() noexcept
    {
        WorkerThread::UniqueLock lock{_worker.mutex};
        // TODO add Sync command and use that to sync

        auto mouseQueueFinished    = _nextMouseAction;
        auto keyboardQueueFinished = _nextKeyboardAction;
        for (auto const &mouseAction : _mouseActions)
        {
            mouseQueueFinished += mouseAction.cooldown;
        }
        for (auto const &keyboardAction : _keyboardActions)
        {
            keyboardQueueFinished += keyboardAction.cooldown;
        }

        if (!_mouseActions.empty() && !_keyboardActions.empty())
        {
            if (mouseQueueFinished > keyboardQueueFinished)
            {
                auto const diff = mouseQueueFinished - keyboardQueueFinished;
                _keyboardActions.emplace_back(KeyboardAction::Type::None, Ms{diff}, Key::Return);
            }
            else
            {
                auto const diff = keyboardQueueFinished - mouseQueueFinished;
                _mouseActions.emplace_back(MouseAction::Type::None, Ms{diff}, MouseButton::Left, 0U, 0U, 10U, 10U);
            }
        }
        else if (_mouseActions.empty())
        {
            _nextMouseAction = keyboardQueueFinished;
            _mouseActions.emplace_back(MouseAction::Type::None, 0U, MouseButton::Left, 0U, 0U, 10U, 10U);
        }
        else if (_keyboardActions.empty())
        {
            _nextKeyboardAction = mouseQueueFinished;
            _keyboardActions.emplace_back(KeyboardAction::Type::None, 0U, Key::Return);
        }
    }

    /// @brief Adds a move command to the mouse queue.
    ///
    /// @param targetArea The target area on the screen.
    void moveTo(Rectangle const &targetArea) noexcept
    {
        auto const target = _strategy->calculateTargetIn(targetArea);
        auto const speed  = _strategy->calculateSpeed();
        auto const factor = _strategy->calculateHorizontalSpeedFactor();
        addMouseAction(MouseAction::Type::Move, Ms{0U}, MouseButton::Left, target.x, target.y, speed, factor);
    }

    /// @brief Clicks with the given mouse button.
    ///
    /// @param button The mouse button to click with.
    void click(MouseButton const button) noexcept
    {
        down(button);
        up(button);
    }

    /// @brief Clicks with the given mouse button after moving the cursor to the given position.
    ///
    /// @param button The mouse button to click with.
    /// @param targetArea The target area on the screen.
    void click(MouseButton const button, Rectangle const &targetArea) noexcept
    {
        moveTo(targetArea);
        click(button);
    }

    /// @brief Performs a drag and drop operation.
    ///
    /// @param button The mouse button to click with.
    /// @param startArea The start area on the screen.
    /// @param stopArea The stop area on the screen.
    void dragAndDrop(MouseButton const button, Rectangle const &startArea, Rectangle const &stopArea) noexcept
    {
        moveTo(startArea);
        down(button);
        moveTo(stopArea);
        up(button);
    }

    /// @brief Presses the given mouse button down.
    ///
    /// @param button The button to press down.
    void down(MouseButton const button) noexcept
    {
        addMouseAction(MouseAction::Type::Down, _strategy->calculateButtonDownTime(), button);
    }

    /// @brief Releases the given mouse button.
    ///
    /// @param button The mouse button to release.
    void up(MouseButton const button) noexcept
    {
        addMouseAction(MouseAction::Type::Up, _strategy->calculateButtonUpTime(), button);
    }

    /// @brief Presses the given key down.
    ///
    /// @param key The key to press down.
    void down(Key const key) noexcept
    {
        addKeyboardAction(KeyboardAction::Type::Down, _strategy->calculateKeyDownTime(), key);
    }

    /// @brief Releases the given key.
    ///
    /// @param key The key to release.
    void up(Key const key) noexcept
    {
        addKeyboardAction(KeyboardAction::Type::Up, _strategy->calculateKeyUpTime(), key);
    }

    /// @brief Presses and releases the given key.
    ///
    /// @param key The key to press and release.
    void press(Key const key) noexcept
    {
        down(key);
        up(key);
    }

    /// @brief Enters the given string via the keyboard.
    ///
    /// @param string The string to enter.
    void enterString(std::string_view const string) noexcept {}

private:
    /// @brief The type of clock used in the entire class.
    using Clock = std::chrono::system_clock;

    /// @brief Shortcut to the type of time_point used in the entire class.
    using TimePoint = std::chrono::time_point<Clock>;

    /// @brief Shortcut to milliseconds.
    using Ms = std::chrono::milliseconds;

    /// @brief Data structure for a single mouse related action performed by the emulator.
    struct MouseAction
    {
        /// @brief The type of action.
        enum class Type : std::uint8_t
        {
            /// @brief No action just wait.
            None,

            /// @brief Release key or button.
            Up,

            /// @brief Push key or button.
            Down,

            /// @brief Moves the cursor to a given position.
            Move
        };

        /// @brief The type of the action.
        Type type;

        /// @brief The time to wait after this action before the next one is executed or the interval for the move
        /// command.
        Ms cooldown;

        /// @brief The button of the action.
        MouseButton button;

        /// @brief The x coordinate for move actions.
        std::uint32_t x;

        /// @brief The y coordinate for move actions.
        std::uint32_t y;

        /// @brief The amount of pixels traveled vertically over the interval stored in cooldown.
        double speed;

        /// @brief The factor to get from vertical to horizontal cursor speed.
        double factor;
    };

    /// @brief Data structure for a single keyboard related action performed by the emulator.
    struct KeyboardAction
    {
        /// @brief The type of action.
        enum class Type : std::uint8_t
        {
            /// @brief No action just wait.
            None,

            /// @brief Release key or button.
            Up,

            /// @brief Push key or button.
            Down
        };

        /// @brief The type of the action.
        Type type;

        /// @brief The time to wait after this action before the next one is executed.
        Ms cooldown;

        /// @brief The key of the action.
        Key key;
    };

    /// @brief Emplaces a new MouseAction instance at the back of the _mouseActions queue.
    ///
    /// @param type The type of the action.
    /// @param cooldown The cooldown of the action.
    /// @param button The button of the action (optional).
    /// @param x The x coordinate for move actions (optional).
    /// @param y The y coordinate for move actions (optional).
    /// @param speed The speed for move actions (optional).
    /// @param factor The factor for move actions (optional).
    void addMouseAction(MouseAction::Type const type,
                        Ms const                cooldown,
                        MouseButton const       button = MouseButton::Left,
                        std::uint32_t const     x      = 0U,
                        std::uint32_t const     y      = 0U,
                        double const            speed  = 1000.0,
                        double const            factor = 1.0) noexcept
    {
        WorkerThread::UniqueLock lock{_worker.mutex};
        _mouseActions.emplace_back(type, cooldown, button, x, y, speed, factor);
    }

    /// @brief Emplaces a new KeyboardAction instance at the back of the _keyboardActions queue.
    ///
    /// @param type The type of the action.
    /// @param cooldown The cooldown of the action.
    /// @param key The key of the action (optional).
    void addKeyboardAction(KeyboardAction::Type const type, Ms const cooldown, Key const key = Key::Space) noexcept
    {
        WorkerThread::UniqueLock lock{_worker.mutex};
        _keyboardActions.emplace_back(type, cooldown, key);
    }

    /// @brief The method for the worker thread.
    void threadMethod() noexcept
    {
        WorkerThread::UniqueLock lock{_worker.mutex};
        while (!_worker.shutdownFlag)
        {
            auto const nextTimePoint = nextActionTimePoint();
            if (nextTimePoint > Clock::now())
            {
                _worker.wakeUp.wait_until(lock, nextTimePoint);
            }

            // we need to check again as queues might have been cleared in the mean time
            if (!_mouseActions.empty() && !_keyboardActions.empty())
            {
                if (_nextMouseAction < _nextKeyboardAction)
                {
                    performNextMouseAction();
                }
                else
                {
                    performNextKeyboardAction();
                }
            }
            else if (!_mouseActions.empty())
            {
                performNextMouseAction();
            }
            else if (!_keyboardActions.empty())
            {
                performNextKeyboardAction();
            }
        }
    }

    /// @brief Returns the timepoint of the next action.
    ///
    /// @return The timepoint of the next action.
    TimePoint nextActionTimePoint() const noexcept
    {
        if (_mouseActions.empty() && _keyboardActions.empty())
        {
            return Clock::now() + std::chrono::milliseconds{1000U};
        }
        if (_mouseActions.empty())
        {
            return _nextKeyboardAction;
        }
        if (_keyboardActions.empty())
        {
            return _nextMouseAction;
        }
        return std::min(_nextMouseAction, _nextKeyboardAction);
    }

    /// @brief Executes the next mouse action and removes it from the queue.
    void performNextMouseAction() noexcept
    {
        auto const nextAction = _mouseActions.front();
        _nextMouseAction += nextAction.cooldown;
        switch (nextAction.type)
        {
        case MouseAction::Type::Move: {
            std::uint32_t x;
            std::uint32_t y;
            if (!_interface.getCursorPosition(x, y))
            {
                ++_errorCounter;
            }
            else
            {
                Point currentPosition{};
                // calculate direction
                // calculate distance to target
                //
            }
            break;
        }
        case MouseAction::Type::Down:
            if (!_interface.down(nextAction.button))
            {
                ++_errorCounter;
            }
            _mouseActions.pop_front();
            break;
        case MouseAction::Type::Up:
            if (!_interface.up(nextAction.button))
            {
                ++_errorCounter;
            }
            _mouseActions.pop_front();
            break;
        default:
            // wait
            break;
        }
    }

    /// @brief Executes the next keyboard action and removes it from the queue.
    void performNextKeyboardAction() noexcept
    {
        auto const nextAction = _keyboardActions.front();
        _keyboardActions.pop_front();
        _nextKeyboardAction += nextAction.cooldown;
        switch (nextAction.type)
        {
        case KeyboardAction::Type::Down:
            if (!_interface.down(nextAction.key))
            {
                ++_errorCounter;
            }
            break;
        case KeyboardAction::Type::Up:
            if (!_interface.up(nextAction.key))
            {
                ++_errorCounter;
            }
            break;
        default:
            // wait
            break;
        }
    }

    /// @brief The default deviation strategy if no other is given.
    NormalDeviationStrategy _defaultStrategy;

    /// @brief The pointer to the currently used devation strategy.
    IDeviationStrategy *_strategy{};

    /// @brief The interface used by the emulator.
    TSystemInterface _interface;

    /// @brief The worker thread of the emulator.
    WorkerThread _worker{};

    /// @brief The time point to execute the next mouse action.
    TimePoint _nextMouseAction{};

    /// @brief The list containing all mouse actions to be executed.
    std::deque<MouseAction> _mouseActions{};

    /// @brief The time point to execute the next keyboard action.
    TimePoint _nextKeyboardAction{};

    /// @brief The list containing all keyboard action to be executed.
    std::deque<KeyboardAction> _keyboardActions{};

    /// @brief Counter for the error during the execution.
    std::uint32_t _errorCounter{};
};

} // namespace Terrahertz::Input

#endif // !THZ_AUTO_GAMING_INPUT_EMULATOR_HPP
