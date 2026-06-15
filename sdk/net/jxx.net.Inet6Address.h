#pragma once

#include <vector>

#include "net/jxx.net.InetAddress.h"

namespace jxx::net
{
    class Inet6Address final : public InetAddress
    {
    public:
        Inet6Address(jxx::Ptr<jxx::lang::String> hostName,
                     jxx::Ptr<jxx::lang::String> hostAddress,
                     const std::vector<jxx::lang::jbyte>& bytes,
                     jxx::lang::jint scopeId,
                     jxx::Ptr<NetworkInterface> scopedIf);
        ~Inet6Address() override = default;

    public:
        static jxx::Ptr<Inet6Address> getByAddress(jxx::Ptr<jxx::lang::String> host,
                                                   const jxx::lang::ByteArray addr,
                                                   jxx::lang::jint scopeId);
        static jxx::Ptr<Inet6Address> getByAddress(jxx::Ptr<jxx::lang::String> host,
                                                   const jxx::lang::ByteArray addr,
                                                   jxx::Ptr<NetworkInterface> nif);

        jxx::lang::jint getScopeId() const noexcept;
        jxx::Ptr<NetworkInterface> getScopedInterface() const;
        jxx::lang::jbool isIPv4CompatibleAddress() const;

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

    private:
        jxx::lang::jint scopeId_ = 0;
        jxx::Ptr<NetworkInterface> scopedIf_;
    };
}
