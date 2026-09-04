#include "io/jxx.io.WriteAbortedException.h"

#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.String.h"

namespace jxx::io {

jxx::Ptr<jxx::lang::ClassAny> WriteAbortedException::Class()
{
    return JxxClassInfoMarker::Class();
}

WriteAbortedException::WriteAbortedException()
    : JxxSuper()
{
}

WriteAbortedException::WriteAbortedException(const jxx::Ptr<jxx::lang::String>& message)
    : JxxSuper(message)
{
}

WriteAbortedException::WriteAbortedException(const char* message)
    : JxxSuper(message)
{
}

WriteAbortedException::WriteAbortedException(const std::string& message)
    : JxxSuper(message)
{
}

WriteAbortedException::WriteAbortedException(
    const jxx::Ptr<jxx::lang::String>& message,
    const jxx::Ptr<jxx::lang::Throwable>& detailValue)
    : JxxSuper(message)
    , detail(detailValue)
{
}

jxx::Ptr<jxx::lang::Throwable> WriteAbortedException::getCause() const
{
    return detail;
}

jxx::Ptr<jxx::lang::String> WriteAbortedException::getMessage() const
{
    const auto baseMessage = JxxSuper::getMessage();
    if (detail == nullptr) {
        return baseMessage;
    }
    const auto detailText = detail->toString();
    const std::string value =
        (baseMessage == nullptr ? std::string() : baseMessage->utf8()) +
        "; " +
        (detailText == nullptr ? std::string() : detailText->utf8());
    return jxx::NEW<jxx::lang::String>(value);
}

jxx::Ptr<jxx::lang::Object> WriteAbortedException::cloneImpl() const
{
    return jxx::CAST<jxx::lang::Object>(jxx::NEW<WriteAbortedException>(*this));
}

const char* WriteAbortedException::typeName() const noexcept
{
    return "WriteAbortedException";
}

} // namespace jxx::io
