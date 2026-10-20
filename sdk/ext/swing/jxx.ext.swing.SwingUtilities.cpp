#include "ext/swing/jxx.ext.swing.SwingUtilities.h"

#include "ext/swing/internal/jxx.ext.swing.internal.DispatchQueue.h"
#include "lang/jxx.lang.Error.h"
#include "lang/jxx.lang.InvocationTargetException.h"
#include "lang/jxx.lang.NullPointerException.h"
#include "lang/jxx.lang.Throwable.h"

namespace jxx::ext::swing {

void SwingUtilities::invokeLater(
    const ::jxx::Ptr<::jxx::lang::Runnable>& runnable)
{
    if (runnable == nullptr)
        throw ::jxx::lang::NullPointerException("runnable");
    internal::DispatchQueue::instance().invokeLater(
        [runnable] { runnable->run(); });
}

void SwingUtilities::invokeAndWait(
    const ::jxx::Ptr<::jxx::lang::Runnable>& runnable)
{
    if (runnable == nullptr)
        throw ::jxx::lang::NullPointerException("runnable");
    if (isEventDispatchThread())
        throw ::jxx::lang::Error(
            "Cannot call invokeAndWait from the event dispatch thread");
    try {
        internal::DispatchQueue::instance().invokeAndWait(
            [runnable] { runnable->run(); });
    }
    catch (const ::jxx::lang::Throwable& thrown) {
        throw ::jxx::lang::InvocationTargetException(
            thrown.cloneThrowable());
    }
}

::jxx::lang::jbool SwingUtilities::isEventDispatchThread()
{
    return internal::DispatchQueue::instance().isDispatchThread();
}

} // namespace jxx::ext::swing
