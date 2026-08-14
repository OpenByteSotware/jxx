#pragma once

#include <vector>
#if defined(_WIN32)
#include <winsock2.h>
#else
#include <arpa/inet.h>
#endif

#include "net/jxx.net.InetAddress.h"

namespace jxx::net
{
    class Inet4Address final : public InetAddress
    {
    public:
        Inet4Address(const jxx::Ptr<jxx::lang::String> hostName,
                     const jxx::Ptr<jxx::lang::String> hostAddress,
                     const jxx::lang::ByteArray bytes);
        ~Inet4Address() override = default;

    public:
        jxx::lang::jbool isMulticastAddress() const override;
        jxx::lang::jbool isAnyLocalAddress() const override;
        jxx::lang::jbool isLoopbackAddress() const override;
        jxx::lang::jbool isLinkLocalAddress() const override;
        jxx::lang::jbool isSiteLocalAddress() const override;
        jxx::lang::jbool isMCGlobal() const override;
        jxx::lang::jbool isMCNodeLocal() const override;
        jxx::lang::jbool isMCLinkLocal() const override;
        jxx::lang::jbool isMCSiteLocal() const override;
        jxx::lang::jbool isMCOrgLocal() const override;
    };
}
