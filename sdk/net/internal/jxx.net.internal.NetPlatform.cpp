#include "net/internal/jxx.net.internal.NetPlatform.h"

#include <mutex>

namespace jxx::net::internal
{
#if defined(_WIN32)
    namespace
    {
        std::once_flag g_once;
    }
#endif

    void ensureNetworkInitialized()
    {
    #if defined(_WIN32)
        std::call_once(g_once, [] {
            WSADATA wsa{};
            ::WSAStartup(MAKEWORD(2, 2), &wsa);
        });
    #endif
    }

    void shutdownNetworkIfNeeded()
    {
        // Intentionally a no-op at this layer. For library-style usage, we keep
        // WinSock initialized for process lifetime once started.
    }

    void closeNativeSocket(NativeSocket s) noexcept
    {
        if (s == kInvalidSocket)
            return;
    #if defined(_WIN32)
        ::closesocket(s);
    #else
        ::close(s);
    #endif
    }
}
