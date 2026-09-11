#pragma once

#include "lang/jxx.lang.ClassInfoMarker.h"
#include "lang/jxx.lang.RuntimeException.h"
#include "lang/jxx_types.h"

#ifdef code
#undef code
#endif
#ifdef PARSE_ERR
#undef PARSE_ERR
#endif
#ifdef SERIALIZE_ERR
#undef SERIALIZE_ERR
#endif

namespace jxx::lang {
class ClassAny;
class Object;
class String;
template <typename Derived, typename JxxSuper, typename... JxxInterfaces>
class ClassInfo;
} // namespace jxx::lang

namespace jxx::org::w3c::dom::ls {

class LSException : public ::jxx::lang::RuntimeException {
public:
    using JxxSuper = ::jxx::lang::RuntimeException;
    using JxxClassInfoMarker = ::jxx::lang::ClassInfo<LSException, JxxSuper>;

    static ::jxx::Ptr<::jxx::lang::ClassAny> Class();

    enum : ::jxx::lang::jshort {
        PARSE_ERR = 81,
        SERIALIZE_ERR = 82
    };

    ::jxx::lang::jshort code;

    LSException(
        ::jxx::lang::jshort exceptionCode,
        const ::jxx::Ptr<::jxx::lang::String>& message);
    LSException(const LSException& other);
    LSException(LSException&& other) noexcept;
    LSException& operator=(const LSException& other);
    LSException& operator=(LSException&& other) noexcept;
    ~LSException() override;

protected:
    ::jxx::Ptr<::jxx::lang::Object> cloneImpl() const override;
    const char* typeName() const noexcept override;
};

} // namespace jxx::org::w3c::dom::ls
