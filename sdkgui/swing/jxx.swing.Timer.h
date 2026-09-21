#pragma once

#include <atomic>
#include <condition_variable>
#include <mutex>
#include <thread>
#include <vector>

#include "awt/event/jxx.awt.event.ActionListener.h"
#include "io/jxx.io.SerializableI.h"
#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.String.h"

namespace jxx::swing
{
    class Timer : public ::jxx::lang::ClassBase<
        Timer, ::jxx::lang::Object, ::jxx::io::SerializableI>
    {
    public:
        using JxxSuper = ::jxx::lang::Object;
        using Super = ::jxx::lang::ClassBase<
            Timer, JxxSuper, ::jxx::io::SerializableI>;

        Timer(
            ::jxx::lang::jint delay,
            const ::jxx::Ptr<::jxx::awt::event::ActionListener>& listener);
        ~Timer() override;

        void addActionListener(
            const ::jxx::Ptr<::jxx::awt::event::ActionListener>& listener);
        void removeActionListener(
            const ::jxx::Ptr<::jxx::awt::event::ActionListener>& listener);
        void setDelay(::jxx::lang::jint delay);
        ::jxx::lang::jint getDelay() const;
        void setInitialDelay(::jxx::lang::jint delay);
        ::jxx::lang::jint getInitialDelay() const;
        void setRepeats(::jxx::lang::jbool repeats);
        ::jxx::lang::jbool isRepeats() const;
        void setCoalesce(::jxx::lang::jbool coalesce);
        ::jxx::lang::jbool isCoalesce() const;
        void setActionCommand(
            const ::jxx::Ptr<::jxx::lang::String>& command);
        ::jxx::Ptr<::jxx::lang::String> getActionCommand() const;
        void start();
        void restart();
        void stop();
        ::jxx::lang::jbool isRunning() const;
        void fireActionPerformed();

        void writeObject(
            const ::jxx::Ptr<::jxx::io::ObjectOutputStream>& out) override;
        void readObject(
            const ::jxx::Ptr<::jxx::io::ObjectInputStream>& in) override;
        void readObjectNoData() override;

    private:
        void runLoop();
        static void validateDelay(::jxx::lang::jint delay);

        mutable std::mutex mutex_;
        std::condition_variable condition_;
        std::vector<::jxx::Ptr<::jxx::awt::event::ActionListener>> listeners_;
        std::thread thread_;
        std::atomic<::jxx::lang::jbool> running_{false};
        ::jxx::lang::jint delay_;
        ::jxx::lang::jint initialDelay_;
        ::jxx::lang::jbool repeats_ = true;
        ::jxx::lang::jbool coalesce_ = true;
        ::jxx::Ptr<::jxx::lang::String> actionCommand_;
    };
}
