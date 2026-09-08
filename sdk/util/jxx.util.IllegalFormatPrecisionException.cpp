#include "util/jxx.util.IllegalFormatPrecisionException.h"

#include <string>

#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.String.h"

namespace jxx::util
{

    jxx::Ptr<jxx::lang::ClassAny>
        IllegalFormatPrecisionException::Class()
    {
        return JxxClassInfoMarker::Class();
    }

    IllegalFormatPrecisionException::
        IllegalFormatPrecisionException(
            jxx::lang::jint precision)
        : IllegalFormatException(
              jxx::NEW<jxx::lang::String>(
                  std::to_string(precision)))
        , precision_(precision)
    {
    }

    jxx::lang::jint
        IllegalFormatPrecisionException::
        getPrecision() const noexcept
    {
        return precision_;
    }

    jxx::Ptr<jxx::lang::Object>
        IllegalFormatPrecisionException::
        cloneImpl() const
    {
        auto result =
            jxx::NEW<
            IllegalFormatPrecisionException>(
                *this);

        return jxx::CAST<
            jxx::lang::Object>(
                result);
    }

    const char*
        IllegalFormatPrecisionException::
        typeName() const noexcept
    {
        return "IllegalFormatPrecisionException";
    }

} // namespace jxx::util