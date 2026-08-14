#include "net/jxx.net.Inet4Address.h"


namespace
{
    inline unsigned char u_(jxx::lang::jbyte b) { return static_cast<unsigned char>(b); }
}

namespace jxx::net
{
    Inet4Address::Inet4Address(const jxx::Ptr<jxx::lang::String> hostName,
                               const jxx::Ptr<jxx::lang::String> hostAddress,
                               const jxx::lang::ByteArray bytes)
        : InetAddress(std::move(hostName), std::move(hostAddress), bytes, AF_INET)
    {
    }

    jxx::lang::jbool Inet4Address::isMulticastAddress() const { return !bytes_->empty() && (u_((*bytes_)[0]) >= 224U && u_((*bytes_)[0]) <= 239U); }
    jxx::lang::jbool Inet4Address::isAnyLocalAddress() const { return bytes_->length == 4 && (*bytes_)[0] == 0 && (*bytes_)[1] == 0 && (*bytes_)[2] == 0 && (*bytes_)[3] == 0; }
    jxx::lang::jbool Inet4Address::isLoopbackAddress() const { return !bytes_->empty() && u_((*bytes_)[0]) == 127U; }
    jxx::lang::jbool Inet4Address::isLinkLocalAddress() const { return bytes_->length == 4 && u_((*bytes_)[0]) == 169U && u_((*bytes_)[1]) == 254U; }
    jxx::lang::jbool Inet4Address::isSiteLocalAddress() const
    {
        return bytes_->length == 4 && (
            u_((*bytes_)[0]) == 10U ||
            (u_((*bytes_)[0]) == 172U && u_((*bytes_)[1]) >= 16U && u_((*bytes_)[1]) <= 31U) ||
            (u_((*bytes_)[0]) == 192U && u_((*bytes_)[1]) == 168U));
    }
    jxx::lang::jbool Inet4Address::isMCGlobal() const { return isMulticastAddress() && !(isMCLinkLocal() || isMCSiteLocal() || isMCOrgLocal()); }
    jxx::lang::jbool Inet4Address::isMCNodeLocal() const { return false; }
    jxx::lang::jbool Inet4Address::isMCLinkLocal() const { return bytes_->length == 4 && u_((*bytes_)[0]) == 224U && u_((*bytes_)[1]) == 0U && u_((*bytes_)[2]) == 0U; }
    jxx::lang::jbool Inet4Address::isMCSiteLocal() const { return bytes_->length == 4 && u_((*bytes_)[0]) == 239U && u_((*bytes_)[1]) == 255U; }
    jxx::lang::jbool Inet4Address::isMCOrgLocal() const { return bytes_->length == 4 && u_((*bytes_)[0]) == 239U && u_((*bytes_)[1]) >= 192U && u_((*bytes_)[1]) <= 195U; }
}
