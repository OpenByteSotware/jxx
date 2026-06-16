#pragma once

#include "lang/jxx_types.h"

#if defined(_WIN32)
    #ifndef NOMINMAX
        #define NOMINMAX
    #endif
    #include <winsock2.h>
    #include <ws2tcpip.h>
    #include <mswsock.h>
#else
    #include <unistd.h>
#endif

namespace jxx::net::internal
{
#if defined(_WIN32)
    using NativeSocket = SOCKET;
    constexpr NativeSocket kInvalidSocket = INVALID_SOCKET;
#else
    using NativeSocket = int;
    constexpr NativeSocket kInvalidSocket = -1;
#endif

    void ensureNetworkInitialized();
    void closeNativeSocket(NativeSocket s) noexcept;
}
