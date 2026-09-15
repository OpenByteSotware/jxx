#include "lang/jxx.lang.ArrayStoreException.h"
#include "lang/jxx.lang.ClassInfo.h"

jxx::Ptr<jxx::lang::ClassAny> jxx::lang::ArrayStoreException::Class()
{
    return JxxClassInfoMarker::Class();
}

jxx::lang::ArrayStoreException::ArrayStoreException(const ::jxx::Ptr<::jxx::lang::String>& message)
    : jxx::lang::ArrayStoreException::JxxSuper(message) {}

jxx::lang::ArrayStoreException::ArrayStoreException(const char* message)
    : jxx::lang::ArrayStoreException::JxxSuper(message) {
}
