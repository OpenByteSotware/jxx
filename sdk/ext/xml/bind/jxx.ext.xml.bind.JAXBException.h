#pragma once
#include "lang/jxx.lang.ClassInfo.h"

#include "lang/jxx.lang.ClassInfoMarker.h"
#include "lang/jxx.lang.Exception.h"

namespace jxx::ext::xml::bind {

class JAXBException
    : public ::jxx::lang::ClassBase<JAXBException, ::jxx::lang::Exception> {
public:
    using JxxSuper = ::jxx::lang::Exception;
    using Super = ::jxx::lang::ClassBase<JAXBException, JxxSuper>;
    using JxxClassInfoMarker =
        ::jxx::lang::ClassInfo<JAXBException, JxxSuper>;

    static ::jxx::Ptr<::jxx::lang::ClassAny> Class();

    JAXBException();
    explicit JAXBException(
        const ::jxx::Ptr<::jxx::lang::String>& message);
    JAXBException(
        const ::jxx::Ptr<::jxx::lang::String>& message,
        const ::jxx::Ptr<::jxx::lang::Throwable>& cause);
    JAXBException(const JAXBException&) = default;
    JAXBException(JAXBException&&) noexcept = default;
    JAXBException& operator=(const JAXBException&) = default;
    JAXBException& operator=(JAXBException&&) noexcept = default;
    ~JAXBException() override = default;

protected:
    ::jxx::Ptr<::jxx::lang::Object> cloneImpl() const override;
    const char* typeName() const noexcept override;
};

} // namespace jxx::ext::xml::bind
