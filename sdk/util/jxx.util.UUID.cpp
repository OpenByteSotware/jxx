#include "util/jxx.util.UUID.h"

#include <array>
#include <iomanip>
#include <random>
#include <sstream>
#include <vector>

namespace jxx::util {
namespace {
std::uint64_t bits(::jxx::lang::jlong value) {
    return static_cast<std::uint64_t>(value);
}
::jxx::lang::jlong signedBits(std::uint64_t value) {
    return static_cast<::jxx::lang::jlong>(value);
}
std::uint64_t parseHex(const std::string& text) {
    if (text.empty() || text.size() > 16) throw ::jxx::lang::IllegalArgumentException();
    std::uint64_t value=0;
    for(char c:text){value<<=4;if(c>='0'&&c<='9')value|=c-'0';else if(c>='a'&&c<='f')value|=c-'a'+10;else if(c>='A'&&c<='F')value|=c-'A'+10;else throw ::jxx::lang::IllegalArgumentException();}
    return value;
}
std::array<std::uint8_t,16> md5(const ::jxx::lang::ByteArray& input) {
    // Compact RFC 1321 implementation used only by nameUUIDFromBytes.
    static constexpr std::uint32_t s[64]={7,12,17,22,7,12,17,22,7,12,17,22,7,12,17,22,5,9,14,20,5,9,14,20,5,9,14,20,5,9,14,20,4,11,16,23,4,11,16,23,4,11,16,23,4,11,16,23,6,10,15,21,6,10,15,21,6,10,15,21,6,10,15,21};
    static const std::uint32_t k[64]={0xd76aa478,0xe8c7b756,0x242070db,0xc1bdceee,0xf57c0faf,0x4787c62a,0xa8304613,0xfd469501,0x698098d8,0x8b44f7af,0xffff5bb1,0x895cd7be,0x6b901122,0xfd987193,0xa679438e,0x49b40821,0xf61e2562,0xc040b340,0x265e5a51,0xe9b6c7aa,0xd62f105d,0x02441453,0xd8a1e681,0xe7d3fbc8,0x21e1cde6,0xc33707d6,0xf4d50d87,0x455a14ed,0xa9e3e905,0xfcefa3f8,0x676f02d9,0x8d2a4c8a,0xfffa3942,0x8771f681,0x6d9d6122,0xfde5380c,0xa4beea44,0x4bdecfa9,0xf6bb4b60,0xbebfbc70,0x289b7ec6,0xeaa127fa,0xd4ef3085,0x04881d05,0xd9d4d039,0xe6db99e5,0x1fa27cf8,0xc4ac5665,0xf4292244,0x432aff97,0xab9423a7,0xfc93a039,0x655b59c3,0x8f0ccc92,0xffeff47d,0x85845dd1,0x6fa87e4f,0xfe2ce6e0,0xa3014314,0x4e0811a1,0xf7537e82,0xbd3af235,0x2ad7d2bb,0xeb86d391};
    std::vector<std::uint8_t> message;
    if(input) for(std::size_t i=0;i<input->length;++i) message.push_back(static_cast<std::uint8_t>((*input)[i]));
    const std::uint64_t length=message.size()*8;message.push_back(0x80);while(message.size()%64!=56)message.push_back(0);for(int i=0;i<8;++i)message.push_back(static_cast<std::uint8_t>(length>>(8*i)));
    std::uint32_t a0=0x67452301,b0=0xefcdab89,c0=0x98badcfe,d0=0x10325476;
    for(std::size_t off=0;off<message.size();off+=64){std::uint32_t m[16];for(int i=0;i<16;++i)m[i]=message[off+4*i]|message[off+4*i+1]<<8|message[off+4*i+2]<<16|message[off+4*i+3]<<24;auto a=a0,b=b0,c=c0,d=d0;for(int i=0;i<64;++i){std::uint32_t f,g;if(i<16){f=(b&c)|(~b&d);g=i;}else if(i<32){f=(d&b)|(~d&c);g=(5*i+1)%16;}else if(i<48){f=b^c^d;g=(3*i+5)%16;}else{f=c^(b|~d);g=(7*i)%16;}f+=a+k[i]+m[g];a=d;d=c;c=b;b+=((f<<s[i])|(f>>(32-s[i])));}a0+=a;b0+=b;c0+=c;d0+=d;}
    std::array<std::uint8_t,16> out{};std::uint32_t words[4]={a0,b0,c0,d0};for(int i=0;i<4;++i)for(int j=0;j<4;++j)out[i*4+j]=static_cast<std::uint8_t>(words[i]>>(8*j));return out;
}
}
UUID::UUID(::jxx::lang::jlong most,::jxx::lang::jlong least):Super(),mostSigBits_(most),leastSigBits_(least){}
::jxx::Ptr<UUID> UUID::randomUUID(){std::random_device rd;std::mt19937_64 gen(rd());auto most=gen(),least=gen();most=(most&0xffffffffffff0fffULL)|0x0000000000004000ULL;least=(least&0x3fffffffffffffffULL)|0x8000000000000000ULL;return ::jxx::NEW<UUID>(signedBits(most),signedBits(least));}
::jxx::Ptr<UUID> UUID::nameUUIDFromBytes(const ::jxx::lang::ByteArray& name){if(name==nullptr)throw ::jxx::lang::NullPointerException();auto bytes=md5(name);bytes[6]=(bytes[6]&0x0f)|0x30;bytes[8]=(bytes[8]&0x3f)|0x80;std::uint64_t most=0,least=0;for(int i=0;i<8;++i)most=(most<<8)|bytes[i];for(int i=8;i<16;++i)least=(least<<8)|bytes[i];return ::jxx::NEW<UUID>(signedBits(most),signedBits(least));}
::jxx::Ptr<UUID> UUID::fromString(const ::jxx::Ptr<::jxx::lang::String>& name){if(!name)throw ::jxx::lang::NullPointerException();auto value=name->utf8();if(value.size()!=36||value[8]!='-'||value[13]!='-'||value[18]!='-'||value[23]!='-')throw ::jxx::lang::IllegalArgumentException();auto most=(parseHex(value.substr(0,8))<<32)|(parseHex(value.substr(9,4))<<16)|parseHex(value.substr(14,4));auto least=(parseHex(value.substr(19,4))<<48)|parseHex(value.substr(24,12));return ::jxx::NEW<UUID>(signedBits(most),signedBits(least));}
::jxx::lang::jlong UUID::getMostSignificantBits()const{return mostSigBits_;}::jxx::lang::jlong UUID::getLeastSignificantBits()const{return leastSigBits_;}
::jxx::lang::jint UUID::version()const{return static_cast<::jxx::lang::jint>((bits(mostSigBits_)>>12)&0xf);}
::jxx::lang::jint UUID::variant()const{auto value=bits(leastSigBits_);return static_cast<::jxx::lang::jint>((value>>63)==0?0:(value>>62)==2?2:(value>>61)==6?6:7);}
::jxx::lang::jlong UUID::timestamp()const{if(version()!=1)throw ::jxx::lang::UnsupportedOperationException();auto value=bits(mostSigBits_);return static_cast<::jxx::lang::jlong>(((value&0x0fffULL)<<48)|(((value>>16)&0xffffULL)<<32)|(value>>32));}
::jxx::lang::jint UUID::clockSequence()const{if(version()!=1)throw ::jxx::lang::UnsupportedOperationException();return static_cast<::jxx::lang::jint>((bits(leastSigBits_)&0x3fff000000000000ULL)>>48);}
::jxx::lang::jlong UUID::node()const{if(version()!=1)throw ::jxx::lang::UnsupportedOperationException();return static_cast<::jxx::lang::jlong>(bits(leastSigBits_)&0x0000ffffffffffffULL);}
::jxx::lang::jint UUID::compareTo(const ::jxx::Ptr<UUID>&v)const{if(!v)throw ::jxx::lang::NullPointerException();if(mostSigBits_<v->mostSigBits_)return -1;if(mostSigBits_>v->mostSigBits_)return 1;if(leastSigBits_<v->leastSigBits_)return -1;if(leastSigBits_>v->leastSigBits_)return 1;return 0;}
::jxx::lang::jbool UUID::equals(const ::jxx::Ptr<::jxx::lang::Object>&v)const{auto other=::jxx::CAST<UUID>(v);return other&&mostSigBits_==other->mostSigBits_&&leastSigBits_==other->leastSigBits_;}
::jxx::lang::jint UUID::hashCode()const{auto value=bits(mostSigBits_)^bits(leastSigBits_);return static_cast<::jxx::lang::jint>((value>>32)^value);}
::jxx::Ptr<::jxx::lang::String> UUID::toString()const{std::ostringstream out;out<<std::hex<<std::setfill('0')<<std::nouppercase;auto most=bits(mostSigBits_),least=bits(leastSigBits_);out<<std::setw(8)<<(most>>32)<<'-'<<std::setw(4)<<((most>>16)&0xffff)<<'-'<<std::setw(4)<<(most&0xffff)<<'-'<<std::setw(4)<<(least>>48)<<'-'<<std::setw(12)<<(least&0xffffffffffffULL);return ::jxx::NEW<::jxx::lang::String>(out.str());}
void UUID::writeObject(const ::jxx::Ptr<::jxx::io::ObjectOutputStream>&){throw ::jxx::lang::UnsupportedOperationException();}void UUID::readObject(const ::jxx::Ptr<::jxx::io::ObjectInputStream>&){throw ::jxx::lang::UnsupportedOperationException();}void UUID::readObjectNoData(){throw ::jxx::lang::UnsupportedOperationException();}
::jxx::Ptr<::jxx::lang::Object> UUID::cloneImpl()const{throw ::jxx::lang::CloneNotSupportedException();}
} // namespace jxx::util
