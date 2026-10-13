#include <mutex>
#include "net/jxx.net.ClassInitialization.h"


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
#include "net/jxx.net.ContentHandler.h"
#include "net/jxx.net.ContentHandlerFactory.h"
#include "net/jxx.net.DatagramPacket.h"
#include "net/jxx.net.DatagramSocket.h"
#include "net/jxx.net.FileNameMap.h"
#include "net/jxx.net.HttpRetryException.h"
#include "net/jxx.net.IDN.h"
#include "net/jxx.net.MalformedURLException.h"
#include "net/jxx.net.MulticastSocket.h"
#include "net/jxx.net.NoRouteToHostException.h"
#include "net/jxx.net.PortUnreachableException.h"
#include "net/jxx.net.ProtocolException.h"
#include "net/jxx.net.Proxy.h"
#include "net/jxx.net.SocketException.h"
#include "net/jxx.net.SocketImplFactory.h"
#include "net/jxx.net.SocketOptions.h"
#include "net/jxx.net.SocketTimeoutException.h"
#include "net/jxx.net.StandardSocketOptions.h"
#include "net/jxx.net.UnknownHostException.h"
#include "net/jxx.net.UnknownServiceException.h"
#include "net/jxx.net.URISyntaxException.h"
#include "net/jxx.net.URLDecoder.h"
#include "net/jxx.net.URLEncoder.h"
#include "net/jxx.net.URLStreamHandlerFactory.h"

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
        registerType<ContentHandler>();
        registerType<ContentHandlerFactory>();
        registerType<DatagramPacket>();
        registerType<DatagramSocket>();
        registerType<FileNameMap>();
        registerType<HttpRetryException>();
        registerType<IDN>();
        registerType<MalformedURLException>();
        registerType<MulticastSocket>();
        registerType<NoRouteToHostException>();
        registerType<PortUnreachableException>();
        registerType<ProtocolException>();
        registerType<Proxy>();
        registerType<SocketException>();
        registerType<SocketImplFactory>();
        registerType<SocketOptions>();
        registerType<SocketTimeoutException>();
        registerType<StandardSocketOptions>();
        registerType<UnknownHostException>();
        registerType<UnknownServiceException>();
        registerType<URISyntaxException>();
        registerType<URLDecoder>();
        registerType<URLEncoder>();
        registerType<URLStreamHandlerFactory>();
        });
}

} // namespace jxx::net
