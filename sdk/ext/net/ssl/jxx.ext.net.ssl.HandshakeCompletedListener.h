#pragma once

#include "lang/jxx.lang.ClassInfo.h"
#include "util/jxx.util.EventListener.h"

namespace jxx::ext::net::ssl {

class HandshakeCompletedEvent;

class HandshakeCompletedListener
    : public ::jxx::lang::InterfaceBase<
          HandshakeCompletedListener,
          ::jxx::util::EventListener> {
public:
    ~HandshakeCompletedListener() override = default;

    virtual void handshakeCompleted(
        const ::jxx::Ptr<HandshakeCompletedEvent>& event) = 0;
};

} // namespace jxx::ext::net::ssl
