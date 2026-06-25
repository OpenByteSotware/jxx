#pragma once

#include <vector>

#include "lang/jxx.lang.Object.h"
#include "util/jxx.util.Enumeration.h"
#include "util/jxx.util.List.h"

namespace jxx::lang 
{
    class String;
}

namespace jxx::net
{
    class InetAddress;
    class InterfaceAddress;

    class NetworkInterface final : public jxx::lang::Object
    {
    public:
        virtual ~NetworkInterface() override = default;

    public:
        static jxx::Ptr<NetworkInterface> getByName(jxx::Ptr<jxx::lang::String> name);
        static jxx::Ptr<NetworkInterface> getByIndex(jxx::lang::jint index);
        static jxx::Ptr<NetworkInterface> getByInetAddress(jxx::Ptr<InetAddress> addr);
        static jxx::Ptr<jxx::util::Enumeration<NetworkInterface>> getNetworkInterfaces();

        jxx::Ptr<jxx::util::Enumeration<InetAddress>> getInetAddresses() const;
        jxx::Ptr<jxx::util::List<jxx::Ptr<InterfaceAddress>>> getInterfaceAddresses() const;
        jxx::Ptr<jxx::util::Enumeration<NetworkInterface>> getSubInterfaces() const;
        jxx::Ptr<NetworkInterface> getParent() const;

        jxx::Ptr<jxx::lang::String> getName() const;
        jxx::Ptr<jxx::lang::String> getDisplayName() const;
        jxx::lang::jint getIndex() const noexcept;

        jxx::lang::jbool isUp() const;
        jxx::lang::jbool isLoopback() const;
        jxx::lang::jbool isPointToPoint() const;
        jxx::lang::jbool supportsMulticast() const;
        jxx::lang::jbool isVirtual() const;
        jxx::lang::jint getMTU() const;
        jxx::lang::ByteArray getHardwareAddress() const;

        jxx::Ptr<jxx::lang::String> toString() const override;
        jxx::lang::jbool equals(jxx::Ptr<jxx::lang::Object> other) const override;
        jxx::lang::jint hashCode() const override;

    
        NetworkInterface() = default;
    private:
        static jxx::Ptr<NetworkInterface> fromName_(const std::string& name,
                                                    jxx::lang::jint indexHint = 0);

    private:
        jxx::Ptr<jxx::lang::String> name_;
        jxx::Ptr<jxx::lang::String> displayName_;
        jxx::lang::jint index_ = 0;
        std::vector<jxx::Ptr<InetAddress>> inetAddresses_;
        std::vector<jxx::Ptr<InterfaceAddress>> interfaceAddresses_;
        jxx::lang::ByteArray hardwareAddr_;
        jxx::lang::jbool isLoopback_ = false;
        jxx::lang::jbool isPtP_ = false;
        jxx::lang::jbool supportsMulticast_ = false;
        jxx::lang::jbool isUp_ = false;
        jxx::lang::jbool isVirtual_ = false;
        jxx::lang::jint mtu_ = 0;
    };
}
