#pragma once

#include "lang/jxx_types.h"
#include "lang/jxx.lang.Object.h"
#include "net/jxx.net.SocketOptions.h"

namespace jxx::io
{
    class FileDescriptor;
    class InputStream;
    class OutputStream;
}

namespace jxx::net
{
    class Socket;
    class ServerSocket;
    class InetAddress;
    class SocketAddress;

    class SocketImpl : public jxx::lang::Object, public SocketOptions
    {
    protected:
        SocketImpl() = default;

    public:
        ~SocketImpl() override = default;

    public:
        virtual void create(jxx::lang::jbool stream) = 0;
        virtual void connect(jxx::Ptr<jxx::lang::String> host,
                             jxx::lang::jint port) = 0;
        virtual void connect(jxx::Ptr<InetAddress> address,
                             jxx::lang::jint port) = 0;
        virtual void connect(jxx::Ptr<SocketAddress> address,
                             jxx::lang::jint timeout) = 0;
        virtual void bind(jxx::Ptr<InetAddress> host,
                          jxx::lang::jint port) = 0;
        virtual void listen(jxx::lang::jint backlog) = 0;
        virtual void accept(jxx::Ptr<SocketImpl> s) = 0;
        virtual jxx::Ptr<jxx::io::InputStream> getInputStream() = 0;
        virtual jxx::Ptr<jxx::io::OutputStream> getOutputStream() = 0;
        virtual jxx::lang::jint available() = 0;
        virtual void close() = 0;
        virtual void sendUrgentData(jxx::lang::jint data) = 0;

        virtual jxx::Ptr<jxx::io::FileDescriptor> getFileDescriptor() const;
        virtual jxx::Ptr<InetAddress> getInetAddress() const;
        virtual jxx::lang::jint getPort() const noexcept;
        virtual jxx::lang::jint getLocalPort() const noexcept;
        virtual void setPerformancePreferences(jxx::lang::jint connectionTime,
                                               jxx::lang::jint latency,
                                               jxx::lang::jint bandwidth);

    protected:
        jxx::Ptr<Socket> socket_;
        jxx::Ptr<ServerSocket> serverSocket_;
        jxx::Ptr<jxx::io::FileDescriptor> fd_;
        jxx::Ptr<InetAddress> address_;
        jxx::lang::jint port_ = 0;
        jxx::lang::jint localport_ = 0;
    };
}
