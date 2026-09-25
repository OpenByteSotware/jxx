#include "security/jxx.security.Provider.h"
#include "lang/jxx.lang.NullPointerException.h"
namespace jxx::security {
Provider::Provider(const ::jxx::Ptr<::jxx::lang::String>& name):name_(name){if(name_==nullptr)throw ::jxx::lang::NullPointerException();}
::jxx::Ptr<::jxx::lang::String> Provider::getName() const{return name_;}
}
