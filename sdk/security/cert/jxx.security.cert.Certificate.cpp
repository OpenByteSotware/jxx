#include "security/cert/jxx.security.cert.Certificate.h"
#include "lang/jxx.lang.NullPointerException.h"
namespace jxx::security::cert {
Certificate::Certificate(const ::jxx::Ptr<::jxx::lang::String>& type):type_(type){if(type_==nullptr)throw ::jxx::lang::NullPointerException();}
::jxx::Ptr<::jxx::lang::String> Certificate::getType()const{return type_;}
}
