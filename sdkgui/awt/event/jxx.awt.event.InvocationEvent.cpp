#include "awt/event/jxx.awt.event.InvocationEvent.h"

#include <chrono>
#include <sstream>

#include "lang/jxx.lang.Exception.h"
#include "lang/jxx.lang.Throwable.h"

namespace jxx::awt::event
{
    namespace
    {
        ::jxx::lang::jlong currentTimeMillis()
        {
            using namespace std::chrono;
            return static_cast<::jxx::lang::jlong>(duration_cast<milliseconds>(
                system_clock::now().time_since_epoch()).count());
        }
    }

    InvocationEvent::InvocationEvent(
        const ::jxx::Ptr<::jxx::lang::Object>& source,
        const ::jxx::Ptr<::jxx::lang::Runnable>& task)
        : InvocationEvent(source, INVOCATION_DEFAULT, task, nullptr, false)
    {
    }

    InvocationEvent::InvocationEvent(
        const ::jxx::Ptr<::jxx::lang::Object>& source,
        const ::jxx::Ptr<::jxx::lang::Runnable>& task,
        const ::jxx::Ptr<::jxx::lang::Object>& completionNotifier,
        ::jxx::lang::jbool catchThrowables)
        : InvocationEvent(source, INVOCATION_DEFAULT, task,
              completionNotifier, catchThrowables)
    {
    }

    InvocationEvent::InvocationEvent(
        const ::jxx::Ptr<::jxx::lang::Object>& source,
        const ::jxx::Ptr<::jxx::lang::Runnable>& task,
        const ::jxx::Ptr<::jxx::lang::Runnable>& listener,
        ::jxx::lang::jbool catchThrowables)
        : InvocationEvent(source, INVOCATION_DEFAULT, task, nullptr,
              catchThrowables)
    {
        listener_ = listener;
    }

    InvocationEvent::InvocationEvent(
        const ::jxx::Ptr<::jxx::lang::Object>& source,
        ::jxx::lang::jint eventId,
        const ::jxx::Ptr<::jxx::lang::Runnable>& task,
        const ::jxx::Ptr<::jxx::lang::Object>& completionNotifier,
        ::jxx::lang::jbool catchThrowables)
        : Super(source, eventId),
          runnable(task),
          notifier(completionNotifier),
          catchExceptions(catchThrowables),
          when_(currentTimeMillis()),
          dispatched_(false)
    {
    }

    void InvocationEvent::dispatch()
    {
        try
        {
            if (runnable != nullptr)
                runnable->run();
        }
        catch (const ::jxx::Ptr<::jxx::lang::Throwable>& throwable)
        {
            throwable_ = throwable;
            if (!catchExceptions)
                throw;
        }
        catch (...)
        {
            dispatched_ = true;
            if (notifier != nullptr)
                notifier->notifyAll();
            if (listener_ != nullptr)
                listener_->run();
            if (!catchExceptions)
                throw;
            return;
        }

        dispatched_ = true;
        if (notifier != nullptr)
            notifier->notifyAll();
        if (listener_ != nullptr)
            listener_->run();
    }

    ::jxx::Ptr<::jxx::lang::Exception> InvocationEvent::getException() const
    {
        return ::jxx::CAST<::jxx::lang::Exception>(throwable_);
    }

    ::jxx::Ptr<::jxx::lang::Throwable> InvocationEvent::getThrowable() const
    {
        return throwable_;
    }

    ::jxx::lang::jlong InvocationEvent::getWhen() const
    {
        return when_;
    }

    ::jxx::lang::jbool InvocationEvent::isDispatched() const
    {
        return dispatched_;
    }

    ::jxx::Ptr<::jxx::lang::String> InvocationEvent::paramString() const
    {
        std::ostringstream out;
        out << "INVOCATION_DEFAULT,runnable="
            << (runnable != nullptr ? "set" : "null")
            << ",notifier=" << (notifier != nullptr ? "set" : "null")
            << ",catchExceptions=" << (catchExceptions ? "true" : "false")
            << ",when=" << when_;
        return ::jxx::NEW<::jxx::lang::String>(out.str());
    }
}
