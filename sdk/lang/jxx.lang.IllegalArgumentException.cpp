#include "lang/jxx.lang.IllegalArgumentException.h"
#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.String.h"
#include "lang/jxx.lang.Throwable.h"

namespace jxx::lang
{

    jxx::Ptr<ClassAny>
        IllegalArgumentException::Class()
    {
        return JxxClassInfoMarker::Class();
    }

    IllegalArgumentException::
        IllegalArgumentException()
        : RuntimeException()
    {
    }

    IllegalArgumentException::
        IllegalArgumentException(
            const jxx::Ptr<String>& message)
        : RuntimeException(message)
    {
    }

    IllegalArgumentException::
        IllegalArgumentException(
            const char* message)
        : RuntimeException(message)
    {
    }

    IllegalArgumentException::
        IllegalArgumentException(
            const std::string& message)
        : RuntimeException(message)
    {
    }

    IllegalArgumentException::
        IllegalArgumentException(
            const jxx::Ptr<String>& message,
            const jxx::Ptr<Throwable>& cause)
        : RuntimeException(
              message,
              cause)
    {
    }

    IllegalArgumentException::
        IllegalArgumentException(
            const jxx::Ptr<Throwable>& cause)
        : RuntimeException(cause)
    {
    }

    jxx::Ptr<Object>
        IllegalArgumentException::cloneImpl() const
    {
        auto result =
            jxx::NEW<
            IllegalArgumentException>(
                *this);

        return jxx::CAST<Object>(
            result);
    }

    const char*
        IllegalArgumentException::typeName()
        const noexcept
    {
        return "IllegalArgumentException";
    }

} // namespace jxx::lang