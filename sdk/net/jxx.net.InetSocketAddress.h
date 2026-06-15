#pragma once

#include "lang/jxx_types.h"
#include "lang/jxx.lang.String.h"
#include "net/jxx.net.SocketAddress.h"

namespace jxx::net
{
    class InetAddress;

    class InetSocketAddress final : public SocketAddress
    {
    public:
        InetSocketAddress(jxx::lang::jint port);
        InetSocketAddress(jxx::Ptr<InetAddress> addr,
                          jxx::lang::jint port);
        InetSocketAddress(jxx::Ptr<jxx::lang::String> hostname,
                          jxx::lang::jint port);
        ~InetSocketAddress() override = default;

    public:
        static jxx::Ptr<InetSocketAddress> createUnresolved(jxx::Ptr<jxx::lang::String> host,
                                                            jxx::lang::jint port);

        jxx::lang::jint getPort() const noexcept;
        jxx::Ptr<InetAddress> getAddress() const;
        jxx::Ptr<jxx::lang::String> getHostName() const;
        jxx::Ptr<jxx::lang::String> getHostString() const;
        jxx::lang::jbool isUnresolved() const noexcept;

        jxx::Ptr<jxx::lang::String> toString() const override;
        jxx::lang::jbool equals(jxx::Ptr<jxx::lang::Object> other) const override;
        jxx::lang::jint hashCode() const override;

    private:
        InetSocketAddress(jxx::Ptr<InetAddress> addr,
                          jxx::Ptr<jxx::lang::String> host,
                          jxx::lang::jint port,
                          jxx::lang::jbool unresolved);

    private:
        jxx::Ptr<InetAddress> addr_;
        jxx::Ptr<jxx::lang::String> host_;
        jxx::lang::jint port_ = 0;
        jxx::lang::jbool unresolved_ = false;
    };
}
