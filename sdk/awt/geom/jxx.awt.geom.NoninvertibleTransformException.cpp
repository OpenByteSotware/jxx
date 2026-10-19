#include "awt/geom/jxx.awt.geom.NoninvertibleTransformException.h"

#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.String.h"

namespace jxx::awt::geom
{
    auto NoninvertibleTransformException::Class()
        -> ::jxx::Ptr<::jxx::lang::ClassAny>
    {
        return JxxClassInfoMarker::Class();
    }

    NoninvertibleTransformException::NoninvertibleTransformException(
        const ::jxx::Ptr<::jxx::lang::String>& message)
        : Super(message)
    {
    }

    auto NoninvertibleTransformException::cloneImpl() const
        -> ::jxx::Ptr<::jxx::lang::Object>
    {
        return ::jxx::CAST<::jxx::lang::Object>(
            ::jxx::NEW<NoninvertibleTransformException>(*this));
    }

    auto NoninvertibleTransformException::typeName() const noexcept
        -> const char*
    {
        return "NoninvertibleTransformException";
    }
}
