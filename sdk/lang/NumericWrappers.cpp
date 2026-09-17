#include "lang/jxx.lang.Byte.h"
#include "lang/jxx.lang.Short.h"
#include "lang/jxx.lang.Integer.h"
#include "lang/jxx.lang.Long.h"
#include "lang/jxx.lang.Float.h"
#include "lang/jxx.lang.Double.h"
#include <array>
#include <cerrno>
#include <cmath>
#include <cstring>
#include <iomanip>
#include <limits>
#include <sstream>
#include <string>
#include <typeindex>
#include "lang/jxx.lang.Class.h"
#include "lang/jxx.lang.NullPointerException.h"
#include "lang/jxx.lang.NumberFormatException.h"
#include "lang/jxx.lang.String.h"
namespace jxx::lang { namespace {
std::string text(const jxx::Ptr<String>& s){if(!s)throw NumberFormatException("null");return s->utf8();}
void radix(jint r){if(r<2||r>36)throw NumberFormatException("radix out of range");}
template<class T> T signedParse(const jxx::Ptr<String>& s,jint r){radix(r);auto v=text(s);if(v.empty())throw NumberFormatException();size_t p=0;try{long long x=std::stoll(v,&p,r);if(p!=v.size()||x<std::numeric_limits<T>::min()||x>std::numeric_limits<T>::max())throw NumberFormatException();return static_cast<T>(x);}catch(const NumberFormatException&){throw;}catch(...){throw NumberFormatException();}}
template<class T> jxx::Ptr<ClassAny> primitive(const char* n){ClassAny::Meta m;m.binaryName=n;m.typeId=typeid(T);m.isPrimitive=true;m.modifiers=0x0001|0x0010|0x0400;return ClassAny::registerClass(m);}
template<class T> jint cmp(T a,T b){return a<b?-1:a>b?1:0;}
int prefixRadix(std::string& v){size_t p=(v.size()&& (v[0]=='+'||v[0]=='-'))?1:0;if(v.compare(p,2,"0x")==0||v.compare(p,2,"0X")==0||v.compare(p,1,"#")==0){v.erase(p,v[p]=='#'?1:2);return 16;}if(v.size()>p+1&&v[p]=='0'){v.erase(p,1);return 8;}return 10;}
}
#define INTEGRAL_IMPL(N,T,PARSE,PNAME) \
const jxx::Ptr<ClassAny> N::TYPE=primitive<T>(PNAME); jxx::Ptr<ClassAny>N::Class(){return JxxClassInfoMarker::Class();}\
N::N(T v):Super(),value_(v){} N::N(const jxx::Ptr<String>&s):N(PARSE(s)){}\
jxx::Ptr<N>N::valueOf(T v){if constexpr(sizeof(T)<=2){static std::array<jxx::Ptr<N>,256> c=[](){std::array<jxx::Ptr<N>,256>a{};for(int i=-128;i<=127;++i)a[i+128]=jxx::NEW<N>(static_cast<T>(i));return a;}();if(v>=-128&&v<=127)return c[static_cast<int>(v)+128];}return jxx::NEW<N>(v);}\
jxx::Ptr<N>N::valueOf(const jxx::Ptr<String>&s){return valueOf(PARSE(s));} jxx::Ptr<N>N::valueOf(const jxx::Ptr<String>&s,jint r){return valueOf(PARSE(s,r));}\
T N::PARSE(const jxx::Ptr<String>&s){return PARSE(s,10);} T N::PARSE(const jxx::Ptr<String>&s,jint r){return signedParse<T>(s,r);}\
jxx::Ptr<N>N::decode(const jxx::Ptr<String>&s){auto v=text(s);int r=prefixRadix(v);return valueOf(signedParse<T>(jxx::NEW<String>(v),r));}\
jxx::Ptr<String>N::toString(T v){return jxx::NEW<String>(std::to_string(static_cast<long long>(v)));} jint N::hashCode(T v){if constexpr(sizeof(T)==8)return static_cast<jint>(v^(v>>32));else return static_cast<jint>(v);} jint N::compare(T a,T b){return cmp(a,b);}\
jbyte N::byteValue()const{return static_cast<jbyte>(value_);} jshort N::shortValue()const{return static_cast<jshort>(value_);} jint N::intValue()const{return static_cast<jint>(value_);} jlong N::longValue()const{return static_cast<jlong>(value_);} jfloat N::floatValue()const{return static_cast<jfloat>(value_);} jdouble N::doubleValue()const{return static_cast<jdouble>(value_);}\
jxx::Ptr<String>N::toString()const{return toString(value_);} jint N::hashCode()const{return hashCode(value_);} jbool N::equals(const jxx::Ptr<Object>&o)const{auto x=jxx::CAST<N>(o);return x&&x->value_==value_;} jint N::compareTo(const jxx::Ptr<N>&o)const{if(!o)throw NullPointerException();return compare(value_,o->value_);}
INTEGRAL_IMPL(Byte,jbyte,parseByte,"byte")
INTEGRAL_IMPL(Short,jshort,parseShort,"short")
INTEGRAL_IMPL(Integer,jint,parseInt,"int")
INTEGRAL_IMPL(Long,jlong,parseLong,"long")
jint Byte::toUnsignedInt(jbyte v){return static_cast<unsigned char>(v);} jlong Byte::toUnsignedLong(jbyte v){return static_cast<unsigned char>(v);} jshort Short::reverseBytes(jshort v){auto u=static_cast<uint16_t>(v);return static_cast<jshort>((u>>8)|(u<<8));}
#undef INTEGRAL_IMPL
#define FP_IMPL(N,T,BT,UT,PNAME,LOW,RAW,FROM) \
const jxx::Ptr<ClassAny>N::TYPE=primitive<T>(PNAME);jxx::Ptr<ClassAny>N::Class(){return JxxClassInfoMarker::Class();}N::N(T v):Super(),value_(v){}N::N(const jxx::Ptr<String>&s):N(parse##N(s)){}jxx::Ptr<N>N::valueOf(T v){return jxx::NEW<N>(v);}jxx::Ptr<N>N::valueOf(const jxx::Ptr<String>&s){return valueOf(parse##N(s));}\
T N::parse##N(const jxx::Ptr<String>&s){try{auto v=text(s);size_t p=0;T x=static_cast<T>(std::stold(v,&p));if(p!=v.size())throw NumberFormatException();return x;}catch(const NumberFormatException&){throw;}catch(...){throw NumberFormatException();}}\
jxx::Ptr<String>N::toString(T v){if(std::isnan(v))return jxx::NEW<String>("NaN");if(std::isinf(v))return jxx::NEW<String>(v<0?"-Infinity":"Infinity");std::ostringstream o;o<<std::setprecision(std::numeric_limits<T>::max_digits10)<<v;return jxx::NEW<String>(o.str());}\
jxx::Ptr<String>N::toHexString(T v){std::ostringstream o;o<<std::hexfloat<<v;return jxx::NEW<String>(o.str());}jbool N::isNaN(T v){return std::isnan(v);}jbool N::isInfinite(T v){return std::isinf(v);}jbool N::isNaN()const{return isNaN(value_);}jbool N::isInfinite()const{return isInfinite(value_);}\
BT N::LOW##To##RAW##Bits(T v){if(std::isnan(v))v=std::numeric_limits<T>::quiet_NaN();return LOW##ToRaw##RAW##Bits(v);}BT N::LOW##ToRaw##RAW##Bits(T v){UT u;std::memcpy(&u,&v,sizeof u);return static_cast<BT>(u);}T N::FROM##BitsTo##N(BT b){UT u=static_cast<UT>(b);T v;std::memcpy(&v,&u,sizeof v);return v;}\
jint N::hashCode(T v){auto b=LOW##To##RAW##Bits(v);if constexpr(sizeof(T)==4)return static_cast<jint>(b);else return static_cast<jint>(b^(b>>32));}jint N::compare(T a,T b){if(a<b)return -1;if(a>b)return 1;auto x=LOW##To##RAW##Bits(a),y=LOW##To##RAW##Bits(b);return x==y?0:x<y?-1:1;}\
jbyte N::byteValue()const{return static_cast<jbyte>(value_);}jshort N::shortValue()const{return static_cast<jshort>(value_);}jint N::intValue()const{return static_cast<jint>(value_);}jlong N::longValue()const{return static_cast<jlong>(value_);}jfloat N::floatValue()const{return static_cast<jfloat>(value_);}jdouble N::doubleValue()const{return static_cast<jdouble>(value_);}jxx::Ptr<String>N::toString()const{return toString(value_);}jint N::hashCode()const{return hashCode(value_);}jbool N::equals(const jxx::Ptr<Object>&o)const{auto x=jxx::CAST<N>(o);return x&&LOW##To##RAW##Bits(value_)==LOW##To##RAW##Bits(x->value_);}jint N::compareTo(const jxx::Ptr<N>&o)const{if(!o)throw NullPointerException();return compare(value_,o->value_);}
FP_IMPL(Float,jfloat,jint,uint32_t,"float",float,Int,int)
FP_IMPL(Double,jdouble,jlong,uint64_t,"double",double,Long,long)
#undef FP_IMPL
} // namespace jxx::lang
