#include "lang/jxx.lang.ArrayIndexOutOfBoundsException.h"
#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.String.h"

#include <string>

jxx::Ptr<jxx::lang::ClassAny> jxx::lang::ArrayIndexOutOfBoundsException::Class()
{
    return JxxClassInfoMarker::Class();
}

jxx::lang::ArrayIndexOutOfBoundsException::ArrayIndexOutOfBoundsException(const ::jxx::Ptr<::jxx::lang::String>& message)
    : jxx::lang::ArrayIndexOutOfBoundsException::JxxSuper(message) {}

jxx::lang::ArrayIndexOutOfBoundsException::ArrayIndexOutOfBoundsException(const char* message)
    : jxx::lang::ArrayIndexOutOfBoundsException::JxxSuper(message) {
}


jxx::lang::ArrayIndexOutOfBoundsException::ArrayIndexOutOfBoundsException(
    ::jxx::lang::jint index)
    : jxx::lang::ArrayIndexOutOfBoundsException::JxxSuper(
          ::jxx::NEW<::jxx::lang::String>(
              std::to_string(index))) {
}
