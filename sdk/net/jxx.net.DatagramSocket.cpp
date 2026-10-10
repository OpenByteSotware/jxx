#include "net/jxx.net.DatagramSocketImpl.h"

namespace jxx::net
{
static void validatePort_(::jxx::lang::jint port) {
    if (port < 0 || port > 65535) throw jxx::lang::IllegalArgumentException("port out of range");
}
DatagramSocket::DatagramSocket():impl_(std::make_unique<Impl>()){} DatagramSocket::DatagramSocket(Family f):impl_(std::make_unique<Impl>(f)){}
DatagramSocket::DatagramSocket(::jxx::lang::jint p):DatagramSocket(jxx::NEW<InetSocketAddress>(p)){}
DatagramSocket::DatagramSocket(::jxx::lang::jint p,const jxx::Ptr<InetAddress>&a):DatagramSocket(jxx::NEW<InetSocketAddress>(a,p)){}
DatagramSocket::DatagramSocket(const jxx::Ptr<SocketAddress>&a):impl_(std::make_unique<Impl>()){if(a)bind(a);}
DatagramSocket::DatagramSocket(DatagramSocket&&)noexcept=default; DatagramSocket& DatagramSocket::operator=(DatagramSocket&&)noexcept=default; DatagramSocket::~DatagramSocket()=default;
void DatagramSocket::bind(const jxx::Ptr<SocketAddress>& address) {
    const auto inetAddress = std::dynamic_pointer_cast<InetSocketAddress>(address);
    if (inetAddress == nullptr) {
        throw jxx::lang::IllegalArgumentException("unsupported socket address");
    }
    if (inetAddress->isUnresolved()) {
        throw jxx::lang::IllegalArgumentException("unresolved socket address");
    }
    const auto resolvedAddress = inetAddress->getAddress();
    if (resolvedAddress == nullptr) {
        throw jxx::lang::IllegalArgumentException("socket address has no address");
    }
    impl_->bind(inet_address_to_ip(resolvedAddress),
        static_cast<std::uint16_t>(inetAddress->getPort()));
}
void DatagramSocket::connect(const jxx::Ptr<InetAddress>&a,::jxx::lang::jint p){if(!a)throw jxx::lang::NullPointerException();validatePort_(p);impl_->connect(a->getHostAddress()->utf8(),static_cast<std::uint16_t>(p));}
void DatagramSocket::connect(const jxx::Ptr<SocketAddress>&a){auto i=std::dynamic_pointer_cast<InetSocketAddress>(a);if(!i||i->isUnresolved())throw jxx::lang::IllegalArgumentException("unsupported socket address");connect(i->getAddress(),i->getPort());}
void DatagramSocket::disconnect(){impl_->disconnect();}
static NativeDatagramPacket native_(const jxx::Ptr<DatagramPacket>&p){if(!p)throw jxx::lang::NullPointerException();NativeDatagramPacket n;auto b=p->getData();n.offset=p->getOffset();n.length=p->getLength();n.buffer.resize(b?b->length:0);for(::jxx::lang::jint i=0;b&&i<b->length;++i)n.buffer[i]=static_cast<std::uint8_t>((*b)[i]);auto a=p->getAddressText();n.address=a?a->utf8():std::string();n.port=static_cast<std::uint16_t>(p->getPort());return n;}
void DatagramSocket::send(const jxx::Ptr<DatagramPacket>&p){auto n=native_(p);impl_->send(n);} void DatagramSocket::receive(const jxx::Ptr<DatagramPacket>&p){auto n=native_(p);impl_->receive(n);auto b=p->getData();for(std::size_t i=0;b&&i<n.length&&n.offset+i<static_cast<std::size_t>(b->length);++i)(*b)[static_cast<::jxx::lang::jint>(n.offset+i)]=static_cast<::jxx::lang::jbyte>(n.buffer[n.offset+i]);p->setLength(static_cast<::jxx::lang::jint>(n.length));p->setAddress(InetAddress::getByName(jxx::NEW<jxx::lang::String>(n.address)));p->setPort(n.port);}
void DatagramSocket::close()noexcept{if(impl_)impl_->close();}
jxx::Ptr<InetAddress> DatagramSocket::getInetAddress()const{auto a=impl_->getRemoteAddress();return a.empty()?nullptr:InetAddress::getByName(jxx::NEW<jxx::lang::String>(a));} jxx::Ptr<InetAddress> DatagramSocket::getLocalAddress()const{auto a=impl_->getLocalAddress();return a.empty()?nullptr:InetAddress::getByName(jxx::NEW<jxx::lang::String>(a));}
::jxx::lang::jint DatagramSocket::getPort()const noexcept{return impl_->getRemotePort();} ::jxx::lang::jint DatagramSocket::getLocalPort()const noexcept{return impl_->isBound()?impl_->getLocalPort():-1;}
jxx::Ptr<SocketAddress> DatagramSocket::getRemoteSocketAddress()const{auto a=getInetAddress();return a?jxx::NEW<InetSocketAddress>(a,getPort()):nullptr;} jxx::Ptr<SocketAddress> DatagramSocket::getLocalSocketAddress()const{auto a=getLocalAddress();return a?jxx::NEW<InetSocketAddress>(a,getLocalPort()):nullptr;} jxx::Ptr<jxx::nio::channels::DatagramChannel> DatagramSocket::getChannel()const{return nullptr;}
::jxx::lang::jbool DatagramSocket::isBound()const noexcept{return impl_&&impl_->isBound();} ::jxx::lang::jbool DatagramSocket::isConnected()const noexcept{return impl_&&impl_->isConnected();} ::jxx::lang::jbool DatagramSocket::isClosed()const noexcept{return !impl_||impl_->isClosed();}
#define V(n,t) void DatagramSocket::n(t v){impl_->n(v);} 
V(setSoTimeout,::jxx::lang::jint) V(setSendBufferSize,::jxx::lang::jint) V(setReceiveBufferSize,::jxx::lang::jint) V(setReuseAddress,::jxx::lang::jbool) V(setBroadcast,::jxx::lang::jbool)
#undef V
::jxx::lang::jint DatagramSocket::getSoTimeout()const noexcept{return impl_->getSoTimeout();} ::jxx::lang::jint DatagramSocket::getSendBufferSize()const{return impl_->getSendBufferSize();} ::jxx::lang::jint DatagramSocket::getReceiveBufferSize()const{return impl_->getReceiveBufferSize();} ::jxx::lang::jbool DatagramSocket::getReuseAddress()const noexcept{return impl_->getReuseAddress();} ::jxx::lang::jbool DatagramSocket::getBroadcast()const noexcept{return impl_->getBroadcast();}
void DatagramSocket::setTrafficClass(::jxx::lang::jint t){if(t<0||t>255)throw jxx::lang::IllegalArgumentException("traffic class out of range");impl_->setTrafficClass(t);} ::jxx::lang::jint DatagramSocket::getTrafficClass()const{return impl_->getTrafficClass();}
#define M(n,args,call) void DatagramSocket::n args{impl_->n call;}
M(joinGroupIPv4,(const std::string&a,const std::string&i),(a,i)) M(leaveGroupIPv4,(const std::string&a,const std::string&i),(a,i)) M(setMulticastTTL,(int v),(v)) M(setMulticastLoopIPv4,(bool v),(v)) M(setMulticastInterfaceIPv4,(const std::string&a),(a)) M(joinGroupIPv6,(const std::string&a,unsigned i),(a,i)) M(leaveGroupIPv6,(const std::string&a,unsigned i),(a,i)) M(setMulticastHopsIPv6,(int v),(v)) M(setMulticastLoopIPv6,(bool v),(v)) M(setMulticastInterfaceIPv6,(unsigned i),(i))
#undef M
} // namespace jxx::net
