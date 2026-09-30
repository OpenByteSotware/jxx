#pragma once

#include <vector>

#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.Runnable.h"

namespace jxx::ext::net::ssl {
class HandshakeCompletedEvent;
class HandshakeCompletedListener;
}

namespace jxx::ext::net::ssl::internal {

class HandshakeNotificationTask final
    : public ::jxx::lang::ClassBase<
          HandshakeNotificationTask,
          ::jxx::lang::Object,
          ::jxx::lang::Runnable> {
public:
    using Listener =
        ::jxx::ext::net::ssl::HandshakeCompletedListener;
    using Event =
        ::jxx::ext::net::ssl::HandshakeCompletedEvent;

    HandshakeNotificationTask(
        const std::vector<::jxx::Ptr<Listener>>& listeners,
        const ::jxx::Ptr<Event>& event);

    void run() override;

private:
    std::vector<::jxx::Ptr<Listener>> listeners_;
    ::jxx::Ptr<Event> event_;
};

} // namespace jxx::ext::net::ssl::internal
