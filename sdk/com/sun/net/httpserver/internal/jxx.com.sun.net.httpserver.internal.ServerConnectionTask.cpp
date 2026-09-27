#include "com/sun/net/httpserver/internal/jxx.com.sun.net.httpserver.internal.ServerConnectionTask.h"
#include "lang/jxx.lang.Exceptions.h"

namespace jxx::com::sun::net::httpserver::internal {

ServerConnectionTask::ServerConnectionTask(std::function<void()> action)
    : Super(), action_(std::move(action))
{
    if (!action_) {
        throw ::jxx::lang::NullPointerException();
    }
}

void ServerConnectionTask::run()
{
    action_();
}

} // namespace jxx::com::sun::net::httpserver::internal
