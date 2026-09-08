#pragma once

#include "lang/jxx.lang.ClassInfoMarker.h"
#include "lang/jxx.lang.RuntimeException.h"
#include "lang/jxx_types.h"

#ifdef code
#undef code
#endif
#ifdef INDEX_SIZE_ERR
#undef INDEX_SIZE_ERR
#endif
#ifdef DOMSTRING_SIZE_ERR
#undef DOMSTRING_SIZE_ERR
#endif
#ifdef HIERARCHY_REQUEST_ERR
#undef HIERARCHY_REQUEST_ERR
#endif
#ifdef WRONG_DOCUMENT_ERR
#undef WRONG_DOCUMENT_ERR
#endif
#ifdef INVALID_CHARACTER_ERR
#undef INVALID_CHARACTER_ERR
#endif
#ifdef NO_DATA_ALLOWED_ERR
#undef NO_DATA_ALLOWED_ERR
#endif
#ifdef NO_MODIFICATION_ALLOWED_ERR
#undef NO_MODIFICATION_ALLOWED_ERR
#endif
#ifdef NOT_FOUND_ERR
#undef NOT_FOUND_ERR
#endif
#ifdef NOT_SUPPORTED_ERR
#undef NOT_SUPPORTED_ERR
#endif
#ifdef INUSE_ATTRIBUTE_ERR
#undef INUSE_ATTRIBUTE_ERR
#endif
#ifdef INVALID_STATE_ERR
#undef INVALID_STATE_ERR
#endif
#ifdef SYNTAX_ERR
#undef SYNTAX_ERR
#endif
#ifdef INVALID_MODIFICATION_ERR
#undef INVALID_MODIFICATION_ERR
#endif
#ifdef NAMESPACE_ERR
#undef NAMESPACE_ERR
#endif
#ifdef INVALID_ACCESS_ERR
#undef INVALID_ACCESS_ERR
#endif
#ifdef VALIDATION_ERR
#undef VALIDATION_ERR
#endif
#ifdef TYPE_MISMATCH_ERR
#undef TYPE_MISMATCH_ERR
#endif

namespace jxx::lang {

class ClassAny;
class Object;
class String;

template <
    typename Derived,
    typename JxxSuper,
    typename... JxxInterfaces>
class ClassInfo;

} // namespace jxx::lang

namespace jxx::org::w3c::dom {

class DOMException
    : public ::jxx::lang::RuntimeException {
public:
    using JxxSuper =
        ::jxx::lang::RuntimeException;

    using JxxClassInfoMarker =
        ::jxx::lang::ClassInfo<
            DOMException,
            JxxSuper>;

    static ::jxx::Ptr<::jxx::lang::ClassAny>
    Class();

    enum : ::jxx::lang::jshort {
        INDEX_SIZE_ERR = 1,
        DOMSTRING_SIZE_ERR = 2,
        HIERARCHY_REQUEST_ERR = 3,
        WRONG_DOCUMENT_ERR = 4,
        INVALID_CHARACTER_ERR = 5,
        NO_DATA_ALLOWED_ERR = 6,
        NO_MODIFICATION_ALLOWED_ERR = 7,
        NOT_FOUND_ERR = 8,
        NOT_SUPPORTED_ERR = 9,
        INUSE_ATTRIBUTE_ERR = 10,
        INVALID_STATE_ERR = 11,
        SYNTAX_ERR = 12,
        INVALID_MODIFICATION_ERR = 13,
        NAMESPACE_ERR = 14,
        INVALID_ACCESS_ERR = 15,
        VALIDATION_ERR = 16,
        TYPE_MISMATCH_ERR = 17
    };

    ::jxx::lang::jshort code;

    DOMException(
        ::jxx::lang::jshort exceptionCode,
        const ::jxx::Ptr<::jxx::lang::String>& message);

    DOMException(
        const DOMException& other);

    DOMException(
        DOMException&& other) noexcept;

    DOMException& operator=(
        const DOMException& other);

    DOMException& operator=(
        DOMException&& other) noexcept;

    ~DOMException() override;

protected:
    ::jxx::Ptr<::jxx::lang::Object>
    cloneImpl() const override;

    const char*
    typeName() const noexcept override;
};

} // namespace jxx::org::w3c::dom
