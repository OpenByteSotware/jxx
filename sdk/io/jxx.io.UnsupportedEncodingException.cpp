#include "io/jxx.io.UnsupportedEncodingException.h"

#include "lang/jxx.lang.ClassInfo.h"

namespace jxx::io {

jxx::Ptr<jxx::lang::ClassAny> UnsupportedEncodingException::Class()
{
    return JxxClassInfoMarker::Class();
}

UnsupportedEncodingException::UnsupportedEncodingException()
    : Super()
{
}

UnsupportedEncodingException::UnsupportedEncodingException(const jxx::Ptr<jxx::lang::String>& message)
    : Super(message)
{
}

UnsupportedEncodingException::UnsupportedEncodingException(const char* message)
    : Super(message)
{
}

UnsupportedEncodingException::UnsupportedEncodingException(const std::string& message)
    : Super(message)
{
}

jxx::Ptr<jxx::lang::Object> UnsupportedEncodingException::cloneImpl() const
{
    return jxx::CAST<jxx::lang::Object>(jxx::NEW<UnsupportedEncodingException>(*this));
}

const char* UnsupportedEncodingException::typeName() const noexcept
{
    return "UnsupportedEncodingException";
}

} // namespace jxx::io
