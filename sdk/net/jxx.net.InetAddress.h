#pragma once
#include "lang/jxx.lang.ClassInfo.h"

#include "lang/jxx_types.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.String.h"
#include "lang/jxx.lang.buildin_array.h"

namespace jxx::net {

    class Inet4Address;
    class Inet6Address;
    class NetworkInterface;

    class InetAddress : public jxx::lang::ClassBase<InetAddress, jxx::lang::Object> {
public:
    using JavaSuper = jxx::lang::Object;
    using Super = jxx::lang::ClassBase<InetAddress, jxx::lang::Object>;

    public:
        using InetAddressArray = jxx::JxxArray<jxx::Ptr<InetAddress>, 1U>;

    protected:
        InetAddress(const jxx::Ptr<jxx::lang::String>& hostName,
            const jxx::Ptr<jxx::lang::String>& hostAddress,
            const jxx::lang::ByteArray& bytes,
            jxx::lang::jint family);

    public:
        virtual ~InetAddress() override = default;

    public:
        static jxx::Ptr<InetAddress> getByAddress(const jxx::lang::ByteArray& addr);

        static jxx::Ptr<InetAddress> getByAddress(
            const jxx::Ptr<jxx::lang::String>& host,
            const jxx::lang::ByteArray& addr);
        static jxx::Ptr<InetAddress> getByName(
            const jxx::Ptr<jxx::lang::String>& host);

        static jxx::Ptr<InetAddressArray> getAllByName(
            const jxx::Ptr<jxx::lang::String>& host);

        static jxx::Ptr<InetAddress> getLoopbackAddress();

        static jxx::Ptr<InetAddress> getLocalHost();

        jxx::Ptr<jxx::lang::String> getHostName() const;

        jxx::Ptr<jxx::lang::String> getCanonicalHostName() const;

        virtual jxx::lang::ByteArray getAddress() const;

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

        virtual jxx::lang::jbool isReachable(
            jxx::lang::jint timeout) const;

        virtual jxx::lang::jbool isReachable(
            const jxx::Ptr<NetworkInterface>& netif,
            jxx::lang::jint ttl,
            jxx::lang::jint timeout) const;

        virtual jxx::Ptr<jxx::lang::String> toString() const override;

        virtual jxx::lang::jbool equals(const jxx::Ptr<jxx::lang::Object>& other) const override;

        virtual jxx::lang::jint hashCode() const override;

    protected:
        jxx::lang::jint familyValue_() const noexcept;

        jxx::lang::jbyte byteAt_(jxx::lang::jint index) const;

        jxx::lang::ByteArray rawBytes_() const noexcept;

        static jxx::lang::ByteArray copyAddressBytes_(
            jxx::lang::ByteArray bytes);

    protected:
        jxx::Ptr<jxx::lang::String> hostName_;
        jxx::Ptr<jxx::lang::String> hostAddress_;
        jxx::lang::ByteArray bytes_;
        jxx::lang::jint family_ = 0;
    };

} // namespace jxx::net