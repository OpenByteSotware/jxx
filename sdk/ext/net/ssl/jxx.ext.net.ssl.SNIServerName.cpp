#include "ext/net/ssl/jxx.ext.net.ssl.SNIServerName.h"
#include "lang/jxx.lang.IllegalArgumentException.h"
#include "lang/jxx.lang.NullPointerException.h"
namespace jxx::ext::net::ssl {SNIServerName::SNIServerName(::jxx::lang::jint t,const ::jxx::lang::ByteArray&e):Super(),type_(t),encoded_(e){if(t<0||t>255)throw ::jxx::lang::IllegalArgumentException();if(e==nullptr)throw ::jxx::lang::NullPointerException();}::jxx::lang::jint SNIServerName::getType()const noexcept{return type_;}::jxx::lang::ByteArray SNIServerName::getEncoded()const{auto r=::jxx::NEW<::jxx::lang::JxxArray<::jxx::lang::jbyte,1U>>(encoded_->length);for(::jxx::lang::jint i=0;i<encoded_->length;++i)(*r)[i]=(*encoded_)[i];return r;}}
