#pragma once

#include "awt/jxx.awt.AWTEvent.h"
#include "awt/jxx.awt.ActiveEvent.h"
#include "lang/jxx.lang.Runnable.h"

namespace jxx::lang
{
    class Exception;
    class Throwable;
}

namespace jxx::awt::event
{
    class InvocationEvent :
        public ::jxx::lang::ClassBase<InvocationEvent,
            ::jxx::awt::AWTEvent,
            ::jxx::awt::ActiveEvent>
    {
    public:
        using JxxSuper = ::jxx::awt::AWTEvent;
        using Super = ::jxx::lang::ClassBase<InvocationEvent,
            JxxSuper,
            ::jxx::awt::ActiveEvent>;

        static constexpr ::jxx::lang::jint INVOCATION_FIRST = 1200;
        static constexpr ::jxx::lang::jint INVOCATION_DEFAULT = 1200;
        static constexpr ::jxx::lang::jint INVOCATION_LAST = 1200;

        InvocationEvent(
            const ::jxx::Ptr<::jxx::lang::Object>& source,
            const ::jxx::Ptr<::jxx::lang::Runnable>& runnable);

        InvocationEvent(
            const ::jxx::Ptr<::jxx::lang::Object>& source,
            const ::jxx::Ptr<::jxx::lang::Runnable>& runnable,
            const ::jxx::Ptr<::jxx::lang::Object>& notifier,
            ::jxx::lang::jbool catchThrowables);

        InvocationEvent(
            const ::jxx::Ptr<::jxx::lang::Object>& source,
            const ::jxx::Ptr<::jxx::lang::Runnable>& runnable,
            const ::jxx::Ptr<::jxx::lang::Runnable>& listener,
            ::jxx::lang::jbool catchThrowables);

        void dispatch() override;
        ::jxx::Ptr<::jxx::lang::Exception> getException() const;
        ::jxx::Ptr<::jxx::lang::Throwable> getThrowable() const;
        ::jxx::lang::jlong getWhen() const;
        ::jxx::lang::jbool isDispatched() const;

    protected:
        InvocationEvent(
            const ::jxx::Ptr<::jxx::lang::Object>& source,
            ::jxx::lang::jint id,
            const ::jxx::Ptr<::jxx::lang::Runnable>& runnable,
            const ::jxx::Ptr<::jxx::lang::Object>& notifier,
            ::jxx::lang::jbool catchThrowables);

        ::jxx::Ptr<::jxx::lang::String> paramString() const override;

        ::jxx::Ptr<::jxx::lang::Runnable> runnable;
        ::jxx::Ptr<::jxx::lang::Object> notifier;
        ::jxx::lang::jbool catchExceptions;

    private:
        ::jxx::Ptr<::jxx::lang::Runnable> listener_;
        ::jxx::Ptr<::jxx::lang::Throwable> throwable_;
        ::jxx::lang::jlong when_;
        ::jxx::lang::jbool dispatched_;
    };
}
