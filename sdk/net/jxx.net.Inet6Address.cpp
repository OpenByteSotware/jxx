#include "lang/jxx.lang.Exceptions.h"
#include "net/jxx.net.NetworkInterface.h"
#include "net/jxx.net.Inet6Address.h"

namespace jxx::net {

    namespace {

        static jxx::lang::jint u8(jxx::lang::jbyte b) {
            return static_cast<jxx::lang::jint>(static_cast<unsigned char>(b));
        }

        static void validateIPv6Address(jxx::lang::ByteArray addr) {
            if (addr == nullptr) {
                throw jxx::lang::NullPointerException();
            }

            if (addr->length != 16) {
                throw jxx::lang::IllegalArgumentException();
            }
        }

        static jxx::lang::ByteArray copyBytes(jxx::lang::ByteArray src) {
            validateIPv6Address(src);

            auto copy = jxx::NEW<jxx::lang::ByteArrayType>(16);
            for (jxx::lang::jint i = 0; i < 16; ++i) {
                (*copy)[i] = (*src)[i];
            }
            return copy;
        }

    } // anonymous namespace

    Inet6Address::Inet6Address(jxx::Ptr<jxx::lang::String> hostName,
        jxx::Ptr<jxx::lang::String> hostAddress,
        jxx::lang::ByteArray bytes,
        jxx::lang::jint scopeId,
        jxx::Ptr<NetworkInterface> scopedIf)
        : InetAddress(hostName, hostAddress, bytes, 10)
        , address_(copyBytes(bytes))
        , scopeId_(scopeId)
        , scopedIf_(scopedIf) {}

    jxx::Ptr<Inet6Address> Inet6Address::getByAddress(
        jxx::Ptr<jxx::lang::String> host,
        jxx::lang::ByteArray addr,
        jxx::lang::jint scopeId) {
        validateIPv6Address(addr);

        return jxx::Ptr<Inet6Address>(
            new Inet6Address(host, nullptr, addr, scopeId, nullptr));
    }

    jxx::Ptr<Inet6Address> Inet6Address::getByAddress(
        jxx::Ptr<jxx::lang::String> host,
        jxx::lang::ByteArray addr,
        jxx::Ptr<NetworkInterface> nif) {
        validateIPv6Address(addr);

        return jxx::Ptr<Inet6Address>(
            new Inet6Address(host, nullptr, addr, 0, nif));
    }

    jxx::lang::ByteArray Inet6Address::getAddress() const {
        return copyBytes(address_);
    }

    jxx::lang::jint Inet6Address::getScopeId() const noexcept {
        return scopeId_;
    }

    jxx::Ptr<NetworkInterface> Inet6Address::getScopedInterface() const {
        return scopedIf_;
    }

    jxx::lang::jbool Inet6Address::isIPv4CompatibleAddress() const {
        for (jxx::lang::jint i = 0; i < 12; ++i) {
            if (address_->at(i) != 0) {
                return false;
            }
        }

        return true;
    }

    jxx::lang::jbool Inet6Address::isMulticastAddress() const {
        return static_cast<jxx::lang::jbool>(u8(address_->at(0)) == 0xff);
    }

    jxx::lang::jbool Inet6Address::isAnyLocalAddress() const {
        for (jxx::lang::jint i = 0; i < 16; ++i) {
            if (address_->at(i) != 0) {
                return false;
            }
        }

        return true;
    }

    jxx::lang::jbool Inet6Address::isLoopbackAddress() const {
        for (jxx::lang::jint i = 0; i < 15; ++i) {
            if (address_->at(i) != 0) {
                return false;
            }
        }

        return static_cast<jxx::lang::jbool>(u8(address_->at(15)) == 1);
    }

    jxx::lang::jbool Inet6Address::isLinkLocalAddress() const {
        return static_cast<jxx::lang::jbool>(
            u8(address_->at(0)) == 0xfe &&
            (u8(address_->at(1)) & 0xc0) == 0x80);
    }

    jxx::lang::jbool Inet6Address::isSiteLocalAddress() const {
        return static_cast<jxx::lang::jbool>(
            u8(address_->at(0)) == 0xfe &&
            (u8(address_->at(1)) & 0xc0) == 0xc0);
    }

    jxx::lang::jbool Inet6Address::isMCGlobal() const {
        return static_cast<jxx::lang::jbool>(
            isMulticastAddress() &&
            (u8(address_->at(1)) & 0x0f) == 0x0e);
    }

    jxx::lang::jbool Inet6Address::isMCNodeLocal() const {
        return static_cast<jxx::lang::jbool>(
            isMulticastAddress() &&
            (u8(address_->at(1)) & 0x0f) == 0x01);
    }

    jxx::lang::jbool Inet6Address::isMCLinkLocal() const {
        return static_cast<jxx::lang::jbool>(
            isMulticastAddress() &&
            (u8(address_->at(1)) & 0x0f) == 0x02);
    }

    jxx::lang::jbool Inet6Address::isMCSiteLocal() const {
        return static_cast<jxx::lang::jbool>(
            isMulticastAddress() &&
            (u8(address_->at(1)) & 0x0f) == 0x05);
    }

    jxx::lang::jbool Inet6Address::isMCOrgLocal() const {
        return static_cast<jxx::lang::jbool>(
            isMulticastAddress() &&
            (u8(address_->at(1)) & 0x0f) == 0x08);
    }

} // namespace jxx::net