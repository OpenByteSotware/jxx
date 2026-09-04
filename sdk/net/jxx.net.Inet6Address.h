#pragma once
#include "lang/jxx.lang.ClassInfo.h"

#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.buildin_array.h"
#include "lang/jxx.lang.String.h"
#include "net/jxx.net.InetAddress.h"

namespace jxx::net {

    class NetworkInterface;

    class Inet6Address final : public jxx::lang::ClassBase<Inet6Address, InetAddress> {
public:
    using JavaSuper = InetAddress;
    using Super = jxx::lang::ClassBase<Inet6Address, InetAddress>;

    private:

    private:
        jxx::lang::ByteArray address_;
        jxx::lang::jint scopeId_ = 0;
        jxx::Ptr<NetworkInterface> scopedIf_;

    public:
        Inet6Address(const jxx::Ptr<jxx::lang::String>& hostName,
            const jxx::Ptr<jxx::lang::String>& hostAddress,
            const jxx::lang::ByteArray& bytes,
            jxx::lang::jint scopeId,
            const jxx::Ptr<NetworkInterface>& scopedIf);

    public:
        virtual ~Inet6Address() override = default;

        static jxx::Ptr<Inet6Address> getByAddress(
            const jxx::Ptr<jxx::lang::String>& host,
            const jxx::lang::ByteArray& addr,
            jxx::lang::jint scopeId);

        static jxx::Ptr<Inet6Address> getByAddress(
            const jxx::Ptr<jxx::lang::String>& host,
            const jxx::lang::ByteArray& addr,
            const jxx::Ptr<NetworkInterface>& nif);

        virtual jxx::lang::ByteArray getAddress() const override;

        jxx::lang::jint getScopeId() const noexcept;
        jxx::Ptr<NetworkInterface> getScopedInterface() const;
        jxx::lang::jbool isIPv4CompatibleAddress() const;

        virtual jxx::lang::jbool isMulticastAddress() const override;
        virtual jxx::lang::jbool isAnyLocalAddress() const override;
        virtual jxx::lang::jbool isLoopbackAddress() const override;
        virtual jxx::lang::jbool isLinkLocalAddress() const override;
        virtual jxx::lang::jbool isSiteLocalAddress() const override;
        virtual jxx::lang::jbool isMCGlobal() const override;
        virtual jxx::lang::jbool isMCNodeLocal() const override;
        virtual jxx::lang::jbool isMCLinkLocal() const override;
        virtual jxx::lang::jbool isMCSiteLocal() const override;
        virtual jxx::lang::jbool isMCOrgLocal() const override;
    };

} // namespace jxx::net