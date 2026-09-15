#pragma once
#include "lang/jxx.lang.ClassInfoMarker.h"
#include "lang/jxx.lang.Error.h"
namespace jxx::lang {
class LinkageError : public Error {
public:
    using JxxSuper = Error;
    using JxxClassInfoMarker = jxx::lang::ClassInfo<LinkageError, JxxSuper>;

    static jxx::Ptr<jxx::lang::ClassAny> Class();

    LinkageError() = default;

    LinkageError(const LinkageError&) = default;
    LinkageError(LinkageError&&) noexcept = default;
    LinkageError& operator=(const LinkageError&) = default;
    LinkageError& operator=(LinkageError&&) noexcept = default;
    ~LinkageError() override = default;

public:

    explicit LinkageError(
        const ::jxx::Ptr<::jxx::lang::String>& message);

    explicit LinkageError(const char* message);

    explicit LinkageError(const ::jxx::Ptr<Throwable>& cause)
        : JxxSuper(cause) {}

    LinkageError(const ::jxx::Ptr<String>& message,
                 const ::jxx::Ptr<Throwable>& cause)
        : JxxSuper(message, cause) {}
protected:
    JXX_OBJECT_CLONE(LinkageError)

    const char* typeName() const noexcept override { return "LinkageError"; }
};
}
