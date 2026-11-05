#include "util/jar/jxx.util.jar.JarException.h"
namespace jxx::util::jar { JarException::JarException()=default;JarException::JarException(const ::jxx::Ptr<::jxx::lang::String>& s):Super(s){}JarException::JarException(const char*s):Super(s){}::jxx::Ptr<::jxx::lang::Object> JarException::cloneImpl()const{return ::jxx::CAST<::jxx::lang::Object>(::jxx::NEW<JarException>(*this));}}
