#include "io/jxx.io.CharConversionException.h"

#include "lang/jxx.lang.ClassInfo.h"

namespace jxx::io {

jxx::Ptr<jxx::lang::ClassAny> CharConversionException::Class()
{
    return JxxClassInfoMarker::Class();
}

CharConversionException::CharConversionException()
    : Super()
{
}

CharConversionException::CharConversionException(const jxx::Ptr<jxx::lang::String>& message)
    : Super(message)
{
}

CharConversionException::CharConversionException(const char* message)
    : Super(message)
{
}

CharConversionException::CharConversionException(const std::string& message)
    : Super(message)
{
}

jxx::Ptr<jxx::lang::Object> CharConversionException::cloneImpl() const
{
    return jxx::CAST<jxx::lang::Object>(jxx::NEW<CharConversionException>(*this));
}

const char* CharConversionException::typeName() const noexcept
{
    return "CharConversionException";
}

} // namespace jxx::io
