#include "ext/net/ssl/internal/jxx.ext.net.ssl.internal.HandshakeNotificationTask.h"

#include "ext/net/ssl/jxx.ext.net.ssl.HandshakeCompletedEvent.h"
#include "ext/net/ssl/jxx.ext.net.ssl.HandshakeCompletedListener.h"

namespace jxx::ext::net::ssl::internal {

HandshakeNotificationTask::HandshakeNotificationTask(
    const std::vector<::jxx::Ptr<Listener>>& listeners,
    const ::jxx::Ptr<Event>& event)
    : listeners_(listeners), event_(event) {
}

void HandshakeNotificationTask::run() {
    for (const auto& listener : listeners_) {
        if (listener == nullptr) continue;
        try {
            listener->handshakeCompleted(event_);
        }
        catch (...) {
            // Listener failures must not invalidate a completed handshake or
            // suppress notification of the remaining listeners.
        }
    }
}

} // namespace jxx::ext::net::ssl::internal
