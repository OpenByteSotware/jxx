#include "io/jxx.io.UnsupportedEncodingException.h"

#include "lang/jxx.lang.ClassInfo.h"

namespace jxx::io {

jxx::Ptr<jxx::lang::ClassAny> UnsupportedEncodingException::Class()
{
    return JxxClassInfoMarker::Class();
}

UnsupportedEncodingException::UnsupportedEncodingException()
    : JxxSuper()
{
}

UnsupportedEncodingException::UnsupportedEncodingException(const jxx::Ptr<jxx::lang::String>& message)
    : JxxSuper(message)
{
}

UnsupportedEncodingException::UnsupportedEncodingException(const char* message)
    : JxxSuper(message)
{
}

UnsupportedEncodingException::UnsupportedEncodingException(const std::string& message)
    : JxxSuper(message)
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
