#include "io/jxx.io.InvalidClassException.h"

#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.String.h"

namespace jxx::io {

jxx::Ptr<jxx::lang::ClassAny> InvalidClassException::Class()
{
    return JxxClassInfoMarker::Class();
}

InvalidClassException::InvalidClassException()
    : JxxSuper()
{
}

InvalidClassException::InvalidClassException(const jxx::Ptr<jxx::lang::String>& message)
    : JxxSuper(message)
{
}

InvalidClassException::InvalidClassException(const char* message)
    : JxxSuper(message)
{
}

InvalidClassException::InvalidClassException(const std::string& message)
    : JxxSuper(message)
{
}

InvalidClassException::InvalidClassException(
    const jxx::Ptr<jxx::lang::String>& reason)
    : JxxSuper(reason)
    , classname(nullptr)
{
}

InvalidClassException::InvalidClassException(
    const jxx::Ptr<jxx::lang::String>& className,
    const jxx::Ptr<jxx::lang::String>& reason)
    : JxxSuper(reason)
    , classname(className)
{
}

jxx::Ptr<jxx::lang::String> InvalidClassException::getMessage() const
{
    const auto baseMessage = JxxSuper::getMessage();
    if (classname == nullptr) {
        return baseMessage;
    }
    const std::string value = classname->utf8() + "; " +
        (baseMessage == nullptr ? std::string() : baseMessage->utf8());
    return jxx::NEW<jxx::lang::String>(value);
}

jxx::Ptr<jxx::lang::Object> InvalidClassException::cloneImpl() const
{
    return jxx::CAST<jxx::lang::Object>(jxx::NEW<InvalidClassException>(*this));
}

const char* InvalidClassException::typeName() const noexcept
{
    return "InvalidClassException";
}

} // namespace jxx::io
