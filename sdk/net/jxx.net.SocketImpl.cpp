#include "net/jxx.net.SocketImpl.h"

namespace jxx::net
{
    jxx::Ptr<jxx::io::FileDescriptor> SocketImpl::getFileDescriptor() const { return fd_; }
    jxx::Ptr<InetAddress> SocketImpl::getInetAddress() const { return address_; }
    jxx::lang::jint SocketImpl::getPort() const noexcept { return port_; }
    jxx::lang::jint SocketImpl::getLocalPort() const noexcept { return localport_; }
    void SocketImpl::setPerformancePreferences(jxx::lang::jint, jxx::lang::jint, jxx::lang::jint) {}
}
