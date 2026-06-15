#pragma once

#include <vector>

#include "lang/jxx_types.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.String.h"
#include "lang/jxx.lang.buildin_array.h"

namespace jxx::net
{
    class Inet4Address;
    class Inet6Address;
    class NetworkInterface;

    class InetAddress : public jxx::lang::Object
    {
    protected:
        InetAddress(jxx::Ptr<jxx::lang::String> hostName,
                    jxx::Ptr<jxx::lang::String> hostAddress,
                    const std::vector<jxx::lang::jbyte>& bytes,
                    jxx::lang::jint family);

    public:
        ~InetAddress() override = default;

    public:
        static jxx::Ptr<InetAddress> getByAddress(const jxx::lang::ByteArray addr);
        static jxx::Ptr<InetAddress> getByAddress(jxx::Ptr<jxx::lang::String> host,
                                                  const jxx::lang::ByteArray addr);
        static jxx::Ptr<InetAddress> getByName(jxx::Ptr<jxx::lang::String> host);
        static jxx::Ptr<jxx::JxxArray<jxx::Ptr<InetAddress>, 1U>> getAllByName(jxx::Ptr<jxx::lang::String> host);
        static jxx::Ptr<InetAddress> getLoopbackAddress();
        static jxx::Ptr<InetAddress> getLocalHost();

        jxx::Ptr<jxx::lang::String> getHostName() const;
        jxx::Ptr<jxx::lang::String> getCanonicalHostName() const;
        jxx::lang::ByteArray getAddress() const;
        jxx::Ptr<jxx::lang::String> getHostAddress() const;

        virtual jxx::lang::jbool isMulticastAddress() const;
        virtual jxx::lang::jbool isAnyLocalAddress() const;
        virtual jxx::lang::jbool isLoopbackAddress() const;
        virtual jxx::lang::jbool isLinkLocalAddress() const;
        virtual jxx::lang::jbool isSiteLocalAddress() const;
        virtual jxx::lang::jbool isMCGlobal() const;
        virtual jxx::lang::jbool isMCNodeLocal() const;
        virtual jxx::lang::jbool isMCLinkLocal() const;
        virtual jxx::lang::jbool isMCSiteLocal() const;
        virtual jxx::lang::jbool isMCOrgLocal() const;

        virtual jxx::lang::jbool isReachable(jxx::lang::jint timeout) const;
        virtual jxx::lang::jbool isReachable(jxx::Ptr<NetworkInterface> netif,
                                             jxx::lang::jint ttl,
                                             jxx::lang::jint timeout) const;

        jxx::Ptr<jxx::lang::String> toString() const override;
        jxx::lang::jbool equals(jxx::Ptr<jxx::lang::Object> other) const override;
        jxx::lang::jint hashCode() const override;

    protected:
        jxx::lang::jint familyValue_() const noexcept;
        const std::vector<jxx::lang::jbyte>& rawBytes_() const noexcept;

    protected:
        jxx::Ptr<jxx::lang::String> hostName_;
        jxx::Ptr<jxx::lang::String> hostAddress_;
        std::vector<jxx::lang::jbyte> bytes_;
        jxx::lang::jint family_ = 0;
    };
}
