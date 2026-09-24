#include "io/jxx.io.UTFDataFormatException.h"

#include "lang/jxx.lang.ClassInfo.h"

namespace jxx::io {

jxx::Ptr<jxx::lang::ClassAny> UTFDataFormatException::Class()
{
    return JxxClassInfoMarker::Class();
}

UTFDataFormatException::UTFDataFormatException()
    : Super()
{
}

UTFDataFormatException::UTFDataFormatException(const jxx::Ptr<jxx::lang::String>& message)
    : Super(message)
{
}

UTFDataFormatException::UTFDataFormatException(const char* message)
    : Super(message)
{
}

UTFDataFormatException::UTFDataFormatException(const std::string& message)
    : Super(message)
{
}

jxx::Ptr<jxx::lang::Object> UTFDataFormatException::cloneImpl() const
{
    return jxx::CAST<jxx::lang::Object>(jxx::NEW<UTFDataFormatException>(*this));
}

const char* UTFDataFormatException::typeName() const noexcept
{
    return "UTFDataFormatException";
}

} // namespace jxx::io
