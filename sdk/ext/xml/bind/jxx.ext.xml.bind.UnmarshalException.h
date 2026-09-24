#pragma once
#include "lang/jxx.lang.ClassInfo.h"

#include "ext/xml/bind/jxx.ext.xml.bind.JAXBException.h"

namespace jxx::ext::xml::bind {

class UnmarshalException
    : public ::jxx::lang::ClassBase<UnmarshalException, JAXBException> {
public:
    using JxxSuper = JAXBException;
    using Super = ::jxx::lang::ClassBase<UnmarshalException, JxxSuper>;
    using JxxClassInfoMarker =
        ::jxx::lang::ClassInfo<UnmarshalException, JxxSuper>;

    static ::jxx::Ptr<::jxx::lang::ClassAny> Class();

    UnmarshalException();
    explicit UnmarshalException(
        const ::jxx::Ptr<::jxx::lang::String>& message);
    UnmarshalException(const UnmarshalException&) = default;
    UnmarshalException(UnmarshalException&&) noexcept = default;
    UnmarshalException& operator=(const UnmarshalException&) = default;
    UnmarshalException& operator=(UnmarshalException&&) noexcept = default;
    ~UnmarshalException() override = default;

protected:
    ::jxx::Ptr<::jxx::lang::Object> cloneImpl() const override;
    const char* typeName() const noexcept override;
};

} // namespace jxx::ext::xml::bind
