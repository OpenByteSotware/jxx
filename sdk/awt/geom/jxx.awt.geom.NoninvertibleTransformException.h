#pragma once

#include "lang/jxx.lang.ClassInfoMarker.h"
#include "lang/jxx.lang.Exception.h"

namespace jxx::lang
{
    class ClassAny;
    class String;
    template <typename Derived, typename JxxSuper, typename... JxxInterfaces>
    class ClassInfo;
}

namespace jxx::awt::geom
{
    class NoninvertibleTransformException
        : public ::jxx::lang::Exception
    {
    public:
        using JxxSuper = ::jxx::lang::Exception;
        using Super = JxxSuper;
        using JxxClassInfoMarker = ::jxx::lang::ClassInfo<
            NoninvertibleTransformException, JxxSuper>;

        static auto Class() -> ::jxx::Ptr<::jxx::lang::ClassAny>;

        explicit NoninvertibleTransformException(
            const ::jxx::Ptr<::jxx::lang::String>& message);

        NoninvertibleTransformException(
            const NoninvertibleTransformException&) = default;
        NoninvertibleTransformException(
            NoninvertibleTransformException&&) noexcept = default;
        auto operator=(const NoninvertibleTransformException&)
            -> NoninvertibleTransformException& = default;
        auto operator=(NoninvertibleTransformException&&) noexcept
            -> NoninvertibleTransformException& = default;
        ~NoninvertibleTransformException() override = default;

    protected:
        auto cloneImpl() const -> ::jxx::Ptr<::jxx::lang::Object> override;
        auto typeName() const noexcept -> const char* override;
    };
}
