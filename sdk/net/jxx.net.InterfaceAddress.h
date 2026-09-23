#pragma once

#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx_types.h"
#include "lang/jxx.lang.Object.h"

namespace jxx::lang {
    class String;
}

namespace jxx::net
{
    class InetAddress;

    class InterfaceAddress final : public jxx::lang::ClassBase<InterfaceAddress, jxx::lang::Object> {
public:
    using JxxSuper = jxx::lang::Object;
    using Super = jxx::lang::ClassBase<InterfaceAddress, JxxSuper>;

    public:
        InterfaceAddress(const jxx::Ptr<InetAddress>& address,
                         const jxx::Ptr<InetAddress>& broadcast,
                         jxx::lang::jshort prefixLength);
        virtual ~InterfaceAddress() override = default;

    public:
        jxx::Ptr<InetAddress> getAddress() const;
        jxx::Ptr<InetAddress> getBroadcast() const;
        jxx::lang::jshort getNetworkPrefixLength() const noexcept;
        jxx::Ptr<jxx::lang::String> toString() const override;
        jxx::lang::jbool equals(
            const jxx::Ptr<jxx::lang::Object>& other) const override;
        jxx::lang::jint hashCode() const override;

    private:
        jxx::Ptr<InetAddress> address_;
        jxx::Ptr<InetAddress> broadcast_;
        jxx::lang::jshort prefixLength_ = 0;
    };
}
