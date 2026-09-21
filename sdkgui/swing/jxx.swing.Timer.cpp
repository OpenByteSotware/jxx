#include "swing/jxx.swing.Timer.h"

#include <algorithm>
#include <chrono>

#include "awt/event/jxx.awt.event.ActionEvent.h"
#include "lang/jxx.lang.IllegalArgumentException.h"

namespace jxx::swing
{
    Timer::Timer(
        ::jxx::lang::jint delay,
        const ::jxx::Ptr<::jxx::awt::event::ActionListener>& listener)
        : Super(), delay_(delay), initialDelay_(delay)
    {
        validateDelay(delay);
        if (listener != nullptr) listeners_.push_back(listener);
    }

    Timer::~Timer()
    {
        stop();
    }

    void Timer::validateDelay(::jxx::lang::jint delay)
    {
        if (delay < 0)
            throw ::jxx::lang::IllegalArgumentException("delay");
    }

    void Timer::addActionListener(
        const ::jxx::Ptr<::jxx::awt::event::ActionListener>& listener)
    {
        if (listener == nullptr) return;
        std::lock_guard<std::mutex> lock(mutex_);
        listeners_.push_back(listener);
    }

    void Timer::removeActionListener(
        const ::jxx::Ptr<::jxx::awt::event::ActionListener>& listener)
    {
        std::lock_guard<std::mutex> lock(mutex_);
        listeners_.erase(
            std::remove(listeners_.begin(), listeners_.end(), listener),
            listeners_.end());
    }

    void Timer::setDelay(::jxx::lang::jint delay)
    {
        validateDelay(delay);
        std::lock_guard<std::mutex> lock(mutex_);
        delay_ = delay;
        condition_.notify_all();
    }

    ::jxx::lang::jint Timer::getDelay() const
    {
        std::lock_guard<std::mutex> lock(mutex_);
        return delay_;
    }

    void Timer::setInitialDelay(::jxx::lang::jint delay)
    {
        validateDelay(delay);
        std::lock_guard<std::mutex> lock(mutex_);
        initialDelay_ = delay;
    }

    ::jxx::lang::jint Timer::getInitialDelay() const
    {
        std::lock_guard<std::mutex> lock(mutex_);
        return initialDelay_;
    }

    void Timer::setRepeats(::jxx::lang::jbool repeats)
    {
        std::lock_guard<std::mutex> lock(mutex_);
        repeats_ = repeats;
    }

    ::jxx::lang::jbool Timer::isRepeats() const
    {
        std::lock_guard<std::mutex> lock(mutex_);
        return repeats_;
    }

    void Timer::setCoalesce(::jxx::lang::jbool coalesce)
    {
        std::lock_guard<std::mutex> lock(mutex_);
        coalesce_ = coalesce;
    }

    ::jxx::lang::jbool Timer::isCoalesce() const
    {
        std::lock_guard<std::mutex> lock(mutex_);
        return coalesce_;
    }

    void Timer::setActionCommand(
        const ::jxx::Ptr<::jxx::lang::String>& command)
    {
        std::lock_guard<std::mutex> lock(mutex_);
        actionCommand_ = command;
    }

    ::jxx::Ptr<::jxx::lang::String> Timer::getActionCommand() const
    {
        std::lock_guard<std::mutex> lock(mutex_);
        return actionCommand_;
    }

    void Timer::start()
    {
        if (running_.exchange(true)) return;
        if (thread_.joinable()) thread_.join();
        thread_ = std::thread([this]() { runLoop(); });
    }

    void Timer::restart()
    {
        stop();
        start();
    }

    void Timer::stop()
    {
        running_.store(false);
        condition_.notify_all();
        if (thread_.joinable()
            && thread_.get_id() != std::this_thread::get_id())
            thread_.join();
    }

    ::jxx::lang::jbool Timer::isRunning() const
    {
        return running_.load();
    }

    void Timer::runLoop()
    {
        ::jxx::lang::jint waitDelay;
        {
            std::lock_guard<std::mutex> lock(mutex_);
            waitDelay = initialDelay_;
        }

        while (running_.load())
        {
            std::unique_lock<std::mutex> lock(mutex_);
            if (condition_.wait_for(
                    lock,
                    std::chrono::milliseconds(waitDelay),
                    [this]() { return !running_.load(); }))
                break;
            const auto repeats = repeats_;
            waitDelay = delay_;
            lock.unlock();

            fireActionPerformed();
            if (!repeats)
            {
                running_.store(false);
                break;
            }
        }
    }

    void Timer::fireActionPerformed()
    {
        std::vector<::jxx::Ptr<::jxx::awt::event::ActionListener>> listeners;
        ::jxx::Ptr<::jxx::lang::String> command;
        {
            std::lock_guard<std::mutex> lock(mutex_);
            listeners = listeners_;
            command = actionCommand_;
        }
        auto event = ::jxx::NEW<::jxx::awt::event::ActionEvent>(
            ::jxx::CAST<::jxx::lang::Object>(thisPtr()),
            ::jxx::awt::event::ActionEvent::ACTION_PERFORMED,
            command);
        for (const auto& listener : listeners)
            if (listener != nullptr) listener->actionPerformed(event);
    }

    void Timer::writeObject(
        const ::jxx::Ptr<::jxx::io::ObjectOutputStream>&)
    {
    }

    void Timer::readObject(
        const ::jxx::Ptr<::jxx::io::ObjectInputStream>&)
    {
        stop();
    }

    void Timer::readObjectNoData()
    {
        stop();
    }
}
