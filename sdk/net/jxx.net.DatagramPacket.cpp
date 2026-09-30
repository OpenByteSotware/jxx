#include "net/jxx.net.DatagramPacket.h"

#include "lang/jxx.lang.IllegalArgumentException.h"
#include "lang/jxx.lang.NullPointerException.h"
#include "lang/jxx.lang.String.h"
#include "net/jxx.net.InetAddress.h"
#include "net/jxx.net.InetSocketAddress.h"

namespace jxx::net {
namespace {
void validatePort_(::jxx::lang::jint port) {
    if (port < 0 || port > 65535) {
        throw ::jxx::lang::IllegalArgumentException("port out of range");
    }
}
} // namespace

DatagramPacket::DatagramPacket(const jxx::lang::ByteArray& b):DatagramPacket(b,0,b?b->length:0){}
DatagramPacket::DatagramPacket(const jxx::lang::ByteArray& b,::jxx::lang::jint n):DatagramPacket(b,0,n){}
DatagramPacket::DatagramPacket(const jxx::lang::ByteArray& b,::jxx::lang::jint o,::jxx::lang::jint n):buffer_(b),offset_(o),length_(n){validateRange_(o,n);}
DatagramPacket::DatagramPacket(const jxx::lang::ByteArray& b,::jxx::lang::jint n,const jxx::Ptr<InetAddress>& a,::jxx::lang::jint p):DatagramPacket(b,0,n,a,p){}
DatagramPacket::DatagramPacket(const jxx::lang::ByteArray& b,::jxx::lang::jint o,::jxx::lang::jint n,const jxx::Ptr<InetAddress>& a,::jxx::lang::jint p):DatagramPacket(b,o,n){setAddress(a);setPort(p);}
DatagramPacket::DatagramPacket(const jxx::lang::ByteArray& b,::jxx::lang::jint n,const jxx::Ptr<SocketAddress>& a):DatagramPacket(b,0,n,a){}
DatagramPacket::DatagramPacket(const jxx::lang::ByteArray& b,::jxx::lang::jint o,::jxx::lang::jint n,const jxx::Ptr<SocketAddress>& a):DatagramPacket(b,o,n){setSocketAddress(a);}
void DatagramPacket::validateRange_(::jxx::lang::jint o,::jxx::lang::jint n)const{if(!buffer_)throw jxx::lang::NullPointerException();if(o<0||n<0||o>buffer_->length||n>buffer_->length-o)throw jxx::lang::IllegalArgumentException("packet range out of bounds");}
jxx::Ptr<InetAddress> DatagramPacket::getAddress()const{return address_;} ::jxx::lang::jint DatagramPacket::getPort()const noexcept{return port_;} jxx::lang::ByteArray DatagramPacket::getData()const{return buffer_;} ::jxx::lang::jint DatagramPacket::getOffset()const noexcept{return offset_;} ::jxx::lang::jint DatagramPacket::getLength()const noexcept{return length_;}
jxx::Ptr<jxx::lang::String> DatagramPacket::getAddressText()const{return address_?address_->getHostAddress():nullptr;}
jxx::Ptr<SocketAddress> DatagramPacket::getSocketAddress()const{return address_?jxx::NEW<InetSocketAddress>(address_,port_):nullptr;}
void DatagramPacket::setAddress(const jxx::Ptr<InetAddress>&a){address_=a;} void DatagramPacket::setPort(::jxx::lang::jint p){validatePort_(p);port_=p;}
void DatagramPacket::setData(const jxx::lang::ByteArray&b){buffer_=b;offset_=0;length_=b?b->length:0;if(!b)throw jxx::lang::NullPointerException();}
void DatagramPacket::setData(const jxx::lang::ByteArray&b,::jxx::lang::jint o,::jxx::lang::jint n){buffer_=b;validateRange_(o,n);offset_=o;length_=n;} void DatagramPacket::setLength(::jxx::lang::jint n){validateRange_(offset_,n);length_=n;}
void DatagramPacket::setSocketAddress(const jxx::Ptr<SocketAddress>&a){auto i=std::dynamic_pointer_cast<InetSocketAddress>(a);if(!i)throw jxx::lang::IllegalArgumentException("unsupported socket address");if(i->isUnresolved())throw jxx::lang::IllegalArgumentException("unresolved socket address");address_=i->getAddress();port_=i->getPort();}


} // namespace jxx::net
