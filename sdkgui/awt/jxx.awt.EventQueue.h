#pragma once

#include <condition_variable>
#include <deque>
#include <mutex>
#include <thread>

#include "awt/jxx.awt.AWTEvent.h"
#include "awt/jxx.awt.SecondaryLoop.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.Runnable.h"

namespace jxx::awt
{
    class EventQueue : public ::jxx::lang::ClassBase<EventQueue, ::jxx::lang::Object>
    {
    public:
        using JxxSuper = ::jxx::lang::Object;
        using Super = ::jxx::lang::ClassBase<EventQueue, JxxSuper>;

        EventQueue();
        ~EventQueue() override;

        virtual void postEvent(const ::jxx::Ptr<AWTEvent>& event);
        virtual ::jxx::Ptr<AWTEvent> getNextEvent();
        virtual ::jxx::Ptr<AWTEvent> peekEvent() const;
        virtual ::jxx::Ptr<AWTEvent> peekEvent(::jxx::lang::jint id) const;
        virtual void push(const ::jxx::Ptr<EventQueue>& newEventQueue);
        virtual ::jxx::Ptr<SecondaryLoop> createSecondaryLoop();

        static ::jxx::Ptr<AWTEvent> getCurrentEvent();
        static ::jxx::lang::jlong getMostRecentEventTime();
        static void invokeLater(const ::jxx::Ptr<::jxx::lang::Runnable>& runnable);
        static void invokeAndWait(const ::jxx::Ptr<::jxx::lang::Runnable>& runnable);
        static ::jxx::lang::jbool isDispatchThread();
        static ::jxx::Ptr<EventQueue> getSystemEventQueue();

    protected:
        virtual void dispatchEvent(const ::jxx::Ptr<AWTEvent>& event);
        virtual void pop();

    private:
        void dispatchLoop();
        mutable std::mutex mutex_;
        std::condition_variable condition_;
        std::deque<::jxx::Ptr<AWTEvent>> events_;
        ::jxx::Ptr<EventQueue> pushedQueue_;
        std::thread dispatchThread_;
        ::jxx::lang::jbool stopping_;
    };
}
