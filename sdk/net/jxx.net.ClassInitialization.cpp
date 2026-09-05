#include "net/jxx.net.ClassInitialization.h"

#include <mutex>

#include "lang/jxx.lang.ClassInfo.h"
#include "net/jxx.net.BindException.h"
#include "net/jxx.net.ConnectException.h"
#include "net/jxx.net.HttpURLConnection.h"
#include "net/jxx.net.Inet4Address.h"
#include "net/jxx.net.Inet6Address.h"
#include "net/jxx.net.InetAddress.h"
#include "net/jxx.net.InetSocketAddress.h"
#include "net/jxx.net.InterfaceAddress.h"
#include "net/jxx.net.JarURLConnection.h"
#include "net/jxx.net.NetPermission.h"
#include "net/jxx.net.NetworkInterface.h"
#include "net/jxx.net.ServerSocket.h"
#include "net/jxx.net.Socket.h"
#include "net/jxx.net.SocketAddress.h"
#include "net/jxx.net.SocketImpl.h"
#include "net/jxx.net.SocketPermission.h"
#include "net/jxx.net.StandardProtocolFamily.h"
#include "net/jxx.net.URI.h"
#include "net/jxx.net.URL.h"
#include "net/jxx.net.URLConnection.h"
#include "net/jxx.net.URLPermission.h"
#include "net/jxx.net.URLStreamHandler.h"

namespace jxx::net {

namespace {

template <typename T>
void registerType()
{
    jxx::lang::class_info_detail::registerClassInfo<T>();
}

} // namespace

void initializeClasses()
{
    static std::once_flag flag;

    std::call_once(
        flag,
        [] {
            registerType<BindException>();
            registerType<ConnectException>();
            registerType<HttpURLConnection>();
            registerType<Inet4Address>();
            registerType<Inet6Address>();
            registerType<InetAddress>();
            registerType<InetSocketAddress>();
            registerType<InterfaceAddress>();
            registerType<JarURLConnection>();
            registerType<NetPermission>();
            registerType<NetworkInterface>();
            registerType<ServerSocket>();
            registerType<Socket>();
            registerType<SocketAddress>();
            registerType<SocketImpl>();
            registerType<SocketPermission>();
            registerType<StandardProtocolFamily>();
            registerType<URI>();
            registerType<URL>();
            registerType<URLConnection>();
            registerType<URLPermission>();
            registerType<URLStreamHandler>();
        });
}

} // namespace jxx::net
