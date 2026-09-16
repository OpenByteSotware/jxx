#include "awt/jxx.awt.EventQueue.h"

#include <atomic>
#include <chrono>

#include "awt/jxx.awt.ActiveEvent.h"
#include "awt/event/jxx.awt.event.InputEvent.h"
#include "awt/event/jxx.awt.event.InvocationEvent.h"
#include "lang/jxx.lang.IllegalArgumentException.h"

namespace jxx::awt
{
    namespace
    {
        thread_local ::jxx::Ptr<AWTEvent> currentEvent;
        thread_local ::jxx::lang::jlong recentEventTime = 0;
        std::thread::id dispatchThreadId;

        class SecondaryLoopImpl final : public ::jxx::lang::ClassBase<SecondaryLoopImpl,
            ::jxx::lang::Object, SecondaryLoop>
        {
        public:
            ::jxx::lang::jbool enter() override
            {
                active_.store(true);
                while (active_.load())
                    std::this_thread::sleep_for(std::chrono::milliseconds(1));
                return true;
            }

            ::jxx::lang::jbool exit() override
            {
                return active_.exchange(false);
            }

        private:
            std::atomic<::jxx::lang::jbool> active_{false};
        };
    }

    EventQueue::EventQueue() : stopping_(false)
    {
        dispatchThread_ = std::thread([this] { dispatchLoop(); });
    }

    EventQueue::~EventQueue()
    {
        {
            std::lock_guard<std::mutex> lock(mutex_);
            stopping_ = true;
        }
        condition_.notify_all();
        if (dispatchThread_.joinable() &&
            dispatchThread_.get_id() != std::this_thread::get_id())
            dispatchThread_.join();
    }

    void EventQueue::postEvent(const ::jxx::Ptr<AWTEvent>& event)
    {
        if (!event) throw ::jxx::lang::IllegalArgumentException("event");
        ::jxx::Ptr<EventQueue> target;
        {
            std::lock_guard<std::mutex> lock(mutex_);
            target = pushedQueue_;
            if (!target) events_.push_back(event);
        }
        if (target) target->postEvent(event);
        else condition_.notify_one();
    }

    ::jxx::Ptr<AWTEvent> EventQueue::getNextEvent()
    {
        std::unique_lock<std::mutex> lock(mutex_);
        condition_.wait(lock, [this] { return stopping_ || !events_.empty(); });
        if (events_.empty()) return nullptr;
        auto event = events_.front();
        events_.pop_front();
        return event;
    }

    ::jxx::Ptr<AWTEvent> EventQueue::peekEvent() const
    {
        std::lock_guard<std::mutex> lock(mutex_);
        return events_.empty() ? nullptr : events_.front();
    }

    ::jxx::Ptr<AWTEvent> EventQueue::peekEvent(::jxx::lang::jint id) const
    {
        std::lock_guard<std::mutex> lock(mutex_);
        for (const auto& event : events_)
            if (event && event->getID() == id) return event;
        return nullptr;
    }

    void EventQueue::push(const ::jxx::Ptr<EventQueue>& queue)
    {
        if (!queue) throw ::jxx::lang::IllegalArgumentException("newEventQueue");
        std::lock_guard<std::mutex> lock(mutex_);
        pushedQueue_ = queue;
    }

    void EventQueue::pop()
    {
        std::lock_guard<std::mutex> lock(mutex_);
        pushedQueue_.reset();
    }

    ::jxx::Ptr<SecondaryLoop> EventQueue::createSecondaryLoop()
    {
        return ::jxx::NEW<SecondaryLoopImpl>();
    }

    void EventQueue::dispatchEvent(const ::jxx::Ptr<AWTEvent>& event)
    {
        auto active = ::jxx::CAST<ActiveEvent>(event);
        if (active) active->dispatch();
    }

    void EventQueue::dispatchLoop()
    {
        dispatchThreadId = std::this_thread::get_id();
        while (true)
        {
            auto event = getNextEvent();
            if (!event)
            {
                std::lock_guard<std::mutex> lock(mutex_);
                if (stopping_) break;
                continue;
            }
            currentEvent = event;
            if (auto input = ::jxx::CAST<::jxx::awt::event::InputEvent>(event))
                recentEventTime = input->getWhen();
            else if (auto invocation = ::jxx::CAST<::jxx::awt::event::InvocationEvent>(event))
                recentEventTime = invocation->getWhen();
            dispatchEvent(event);
            currentEvent.reset();
        }
    }

    ::jxx::Ptr<AWTEvent> EventQueue::getCurrentEvent() { return currentEvent; }
    ::jxx::lang::jlong EventQueue::getMostRecentEventTime() { return recentEventTime; }
    ::jxx::lang::jbool EventQueue::isDispatchThread() { return std::this_thread::get_id() == dispatchThreadId; }

    ::jxx::Ptr<EventQueue> EventQueue::getSystemEventQueue()
    {
        static auto queue = ::jxx::NEW<EventQueue>();
        return queue;
    }

    void EventQueue::invokeLater(const ::jxx::Ptr<::jxx::lang::Runnable>& runnable)
    {
        auto queue = getSystemEventQueue();
        queue->postEvent(::jxx::NEW<::jxx::awt::event::InvocationEvent>(
            ::jxx::CAST<::jxx::lang::Object>(queue), runnable));
    }

    void EventQueue::invokeAndWait(const ::jxx::Ptr<::jxx::lang::Runnable>& runnable)
    {
        if (isDispatchThread())
            throw ::jxx::lang::IllegalArgumentException("dispatch thread");
        auto queue = getSystemEventQueue();
        auto event = ::jxx::NEW<::jxx::awt::event::InvocationEvent>(
            ::jxx::CAST<::jxx::lang::Object>(queue), runnable);
        queue->postEvent(event);
        while (!event->isDispatched())
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
        if (event->getThrowable()) throw event->getThrowable();
    }
}
