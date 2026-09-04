#include "io/jxx.io.IOError.h"

#include "lang/jxx.lang.ClassInfo.h"

namespace jxx::io {

jxx::Ptr<jxx::lang::ClassAny> IOError::Class()
{
    return JxxClassInfoMarker::Class();
}

IOError::IOError(const jxx::Ptr<jxx::lang::Throwable>& cause)
    : JxxSuper(cause == nullptr ? nullptr : cause->getMessage(), cause)
    , cause_(cause)
{
}

jxx::Ptr<jxx::lang::Throwable> IOError::getCause() const
{
    return cause_;
}

jxx::Ptr<jxx::lang::Object> IOError::cloneImpl() const
{
    return jxx::CAST<jxx::lang::Object>(jxx::NEW<IOError>(*this));
}

const char* IOError::typeName() const noexcept
{
    return "IOError";
}

} // namespace jxx::io
