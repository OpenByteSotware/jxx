#pragma once

#include <memory>

#include "lang/jxx_types.h"
#include "lang/jxx.lang.Object.h"
#include "net/internal/jxx.net.internal.NetPlatform.h"

namespace jxx::net
{
    class InetAddress;
    class Socket;
    class SocketAddress;
    class InetSocketAddress;
    class SocketImplFactory;

    namespace internal
    {
        struct NativeSocketState;
    }   
}

namespace jxx::nio::channels
{
    class ServerSocketChannel;
}

namespace jxx::net
{
    class ServerSocket final : public jxx::lang::Object
    {
    public:
        ServerSocket();
        explicit ServerSocket(jxx::lang::jint port);
        ServerSocket(jxx::lang::jint port,
                     jxx::lang::jint backlog);
        ServerSocket(jxx::lang::jint port,
                     jxx::lang::jint backlog,
                     jxx::Ptr<InetAddress> bindAddr);
        ~ServerSocket() override;

    public:
        static void setSocketFactory(jxx::Ptr<SocketImplFactory> fac);

        void bind(jxx::Ptr<SocketAddress> endpoint);
        void bind(jxx::Ptr<SocketAddress> endpoint,
                  jxx::lang::jint backlog);
        jxx::Ptr<InetAddress> getInetAddress() const;
        jxx::lang::jint getLocalPort() const noexcept;
        jxx::Ptr<SocketAddress> getLocalSocketAddress() const;
        jxx::Ptr<Socket> accept();
        void close();
        jxx::Ptr<jxx::nio::channels::ServerSocketChannel> getChannel() const;
        jxx::lang::jbool isBound() const noexcept;
        jxx::lang::jbool isClosed() const noexcept;

        void setSoTimeout(jxx::lang::jint timeout);
        jxx::lang::jint getSoTimeout() const noexcept;
        void setReuseAddress(jxx::lang::jbool on);
        jxx::lang::jbool getReuseAddress() const;
        void setReceiveBufferSize(jxx::lang::jint size);
        jxx::lang::jint getReceiveBufferSize() const;
        void setPerformancePreferences(jxx::lang::jint connectionTime,
                                       jxx::lang::jint latency,
                                       jxx::lang::jint bandwidth);

        jxx::Ptr<jxx::lang::String> toString() const override;

    private:
        void ensureCreated_();
        void setSockOptBool_(jxx::lang::jint level,
                             jxx::lang::jint name,
                             jxx::lang::jbool value) const;
        jxx::lang::jbool getSockOptBool_(jxx::lang::jint level,
                                         jxx::lang::jint name) const;
        void setSockOptInt_(jxx::lang::jint level,
                            jxx::lang::jint name,
                            jxx::lang::jint value) const;
        jxx::lang::jint getSockOptInt_(jxx::lang::jint level,
                                       jxx::lang::jint name) const;

    private:
        std::shared_ptr<internal::NativeSocketState> state_;
        jxx::Ptr<InetAddress> localAddr_;
        jxx::lang::jint localPort_ = 0;
        jxx::lang::jbool bound_ = false;
        jxx::lang::jint soTimeout_ = 0;
    };
}
