#pragma once

#include "lang/jxx.lang.ClassInfoMarker.h"
#include "lang/jxx.lang.Exception.h"

namespace jxx::org::xml::sax {

class SAXException
    : public ::jxx::lang::Exception {
public:
    using JxxSuper = ::jxx::lang::Exception;
    using JxxClassInfoMarker = ::jxx::lang::ClassInfo<SAXException, JxxSuper>;

    static ::jxx::Ptr<::jxx::lang::ClassAny> Class();

    SAXException();
    explicit SAXException(const ::jxx::Ptr<::jxx::lang::String>& message);
    explicit SAXException(const ::jxx::Ptr<::jxx::lang::Exception>& exception);
    SAXException(
        const ::jxx::Ptr<::jxx::lang::String>& message,
        const ::jxx::Ptr<::jxx::lang::Exception>& exception);

    SAXException(const SAXException&) = default;
    SAXException(SAXException&&) noexcept = default;
    SAXException& operator=(const SAXException&) = default;
    SAXException& operator=(SAXException&&) noexcept = default;
    ~SAXException() override;

    ::jxx::Ptr<::jxx::lang::Exception> getException() const;

protected:
    ::jxx::Ptr<::jxx::lang::Object> cloneImpl() const override;
    const char* typeName() const noexcept override;

private:
    ::jxx::Ptr<::jxx::lang::Exception> exception_;
};

} // namespace jxx::org::xml::sax
