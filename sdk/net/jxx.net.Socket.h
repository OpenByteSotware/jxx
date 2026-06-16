#pragma once

#include <memory>

#include "lang/jxx_types.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.String.h"
#include "net/internal/jxx.net.internal.NetPlatform.h"
#include "net/jxx.net.SocketOptions.h"

namespace jxx::io
{
    class InputStream;
    class OutputStream;
}

namespace jxx::nio::channels
{
    class SocketChannel;
}

namespace jxx::net
{
    class InetAddress;
    class SocketAddress;
    class InetSocketAddress;
    class Proxy;
    class SocketImpl;
    class SocketImplFactory;

    namespace internal
    {
        struct NativeSocketState;
    }

    class ServerSocket;

    class Socket final : public jxx::lang::Object
    {
    public:
        Socket();
        explicit Socket(jxx::Ptr<Proxy> proxy);
        Socket(jxx::Ptr<jxx::lang::String> host,
               jxx::lang::jint port);
        Socket(jxx::Ptr<InetAddress> address,
               jxx::lang::jint port);
        Socket(jxx::Ptr<jxx::lang::String> host,
               jxx::lang::jint port,
               jxx::Ptr<InetAddress> localAddr,
               jxx::lang::jint localPort);
        Socket(jxx::Ptr<InetAddress> address,
               jxx::lang::jint port,
               jxx::Ptr<InetAddress> localAddr,
               jxx::lang::jint localPort);
        ~Socket() override;

    public:
        static void setSocketImplFactory(jxx::Ptr<SocketImplFactory> fac);

        void connect(jxx::Ptr<SocketAddress> endpoint);
        void connect(jxx::Ptr<SocketAddress> endpoint,
                     jxx::lang::jint timeout);
        void bind(jxx::Ptr<SocketAddress> bindpoint);

        jxx::Ptr<InetAddress> getInetAddress() const;
        jxx::Ptr<InetAddress> getLocalAddress() const;
        jxx::lang::jint getPort() const noexcept;
        jxx::lang::jint getLocalPort() const noexcept;
        jxx::Ptr<SocketAddress> getRemoteSocketAddress() const;
        jxx::Ptr<SocketAddress> getLocalSocketAddress() const;
        jxx::Ptr<jxx::nio::channels::SocketChannel> getChannel() const;

        jxx::Ptr<jxx::io::InputStream> getInputStream();
        jxx::Ptr<jxx::io::OutputStream> getOutputStream();

        void setTcpNoDelay(jxx::lang::jbool on);
        jxx::lang::jbool getTcpNoDelay() const;
        void setSoLinger(jxx::lang::jbool on,
                         jxx::lang::jint linger);
        jxx::lang::jint getSoLinger() const;
        void sendUrgentData(jxx::lang::jint data);
        void setOOBInline(jxx::lang::jbool on);
        jxx::lang::jbool getOOBInline() const;
        void setSoTimeout(jxx::lang::jint timeout);
        jxx::lang::jint getSoTimeout() const noexcept;
        void setSendBufferSize(jxx::lang::jint size);
        jxx::lang::jint getSendBufferSize() const;
        void setReceiveBufferSize(jxx::lang::jint size);
        jxx::lang::jint getReceiveBufferSize() const;
        void setKeepAlive(jxx::lang::jbool on);
        jxx::lang::jbool getKeepAlive() const;
        void setTrafficClass(jxx::lang::jint tc);
        jxx::lang::jint getTrafficClass() const;
        void setReuseAddress(jxx::lang::jbool on);
        jxx::lang::jbool getReuseAddress() const;

        void shutdownInput();
        void shutdownOutput();
        void close();

        jxx::lang::jbool isConnected() const noexcept;
        jxx::lang::jbool isBound() const noexcept;
        jxx::lang::jbool isClosed() const noexcept;
        jxx::lang::jbool isInputShutdown() const noexcept;
        jxx::lang::jbool isOutputShutdown() const noexcept;

        void setPerformancePreferences(jxx::lang::jint connectionTime,
                                       jxx::lang::jint latency,
                                       jxx::lang::jint bandwidth);

        jxx::Ptr<jxx::lang::String> toString() const override;

    private:
        friend class ServerSocket;
        Socket(internal::NativeSocket handle,
               jxx::Ptr<InetAddress> remoteAddr,
               jxx::lang::jint remotePort,
               jxx::Ptr<InetAddress> localAddr,
               jxx::lang::jint localPort);
        void ensureCreated_(jxx::lang::jbool stream);
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
        jxx::Ptr<InetAddress> remoteAddr_;
        jxx::lang::jint remotePort_ = 0;
        jxx::Ptr<InetAddress> localAddr_;
        jxx::lang::jint localPort_ = 0;
        jxx::lang::jbool connected_ = false;
        jxx::lang::jbool bound_ = false;
        jxx::lang::jint soTimeout_ = 0;
        jxx::Ptr<Proxy> proxy_;
        jxx::Ptr<SocketImpl> impl_;
    };
}
