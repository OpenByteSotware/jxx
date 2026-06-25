#pragma once

#include "lang/jxx_types.h"
#include "lang/jxx.lang.Object.h"

namespace jxx::lang {
    class String;
}

namespace jxx::net
{
    class InetAddress;

    class InterfaceAddress final : public jxx::lang::Object
    {
    public:
        InterfaceAddress(jxx::Ptr<InetAddress> address,
                         jxx::Ptr<InetAddress> broadcast,
                         jxx::lang::jshort prefixLength);
        virtual ~InterfaceAddress() override = default;

    public:
        jxx::Ptr<InetAddress> getAddress() const;
        jxx::Ptr<InetAddress> getBroadcast() const;
        jxx::lang::jshort getNetworkPrefixLength() const noexcept;
        jxx::Ptr<jxx::lang::String> toString() const override;

    private:
        jxx::Ptr<InetAddress> address_;
        jxx::Ptr<InetAddress> broadcast_;
        jxx::lang::jshort prefixLength_ = 0;
    };
}
