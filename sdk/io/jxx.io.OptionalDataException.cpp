#include "io/jxx.io.OptionalDataException.h"

#include "lang/jxx.lang.ClassInfo.h"

namespace jxx::io {

jxx::Ptr<jxx::lang::ClassAny> OptionalDataException::Class()
{
    return JxxClassInfoMarker::Class();
}

OptionalDataException::OptionalDataException(jxx::lang::jint lengthValue)
    : JxxSuper()
    , length(lengthValue)
    , eof(false)
{
}

OptionalDataException::OptionalDataException(jxx::lang::jbool endOfData)
    : JxxSuper()
    , length(0)
    , eof(endOfData)
{
}

jxx::Ptr<jxx::lang::Object> OptionalDataException::cloneImpl() const
{
    return jxx::CAST<jxx::lang::Object>(jxx::NEW<OptionalDataException>(*this));
}

const char* OptionalDataException::typeName() const noexcept
{
    return "OptionalDataException";
}

} // namespace jxx::io
