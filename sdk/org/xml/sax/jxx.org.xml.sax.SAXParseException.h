#pragma once

#include "org/xml/sax/jxx.org.xml.sax.SAXException.h"

namespace jxx::org::xml::sax {

class SAXParseException
    : public SAXException {
public:
    using JxxSuper = SAXException;
    using JxxClassInfoMarker = ::jxx::lang::ClassInfo<SAXParseException, JxxSuper>;

    static ::jxx::Ptr<::jxx::lang::ClassAny> Class();

    SAXParseException(
        const ::jxx::Ptr<::jxx::lang::String>& message,
        const ::jxx::Ptr<::jxx::lang::String>& publicId,
        const ::jxx::Ptr<::jxx::lang::String>& systemId,
        ::jxx::lang::jint lineNumber,
        ::jxx::lang::jint columnNumber);

    SAXParseException(const SAXParseException&) = default;
    SAXParseException(SAXParseException&&) noexcept = default;
    SAXParseException& operator=(const SAXParseException&) = default;
    SAXParseException& operator=(SAXParseException&&) noexcept = default;
    ~SAXParseException() override;

    ::jxx::Ptr<::jxx::lang::String> getPublicId() const;
    ::jxx::Ptr<::jxx::lang::String> getSystemId() const;
    ::jxx::lang::jint getLineNumber() const;
    ::jxx::lang::jint getColumnNumber() const;

protected:
    ::jxx::Ptr<::jxx::lang::Object> cloneImpl() const override;
    const char* typeName() const noexcept override;

private:
    ::jxx::Ptr<::jxx::lang::String> publicId_;
    ::jxx::Ptr<::jxx::lang::String> systemId_;
    ::jxx::lang::jint lineNumber_;
    ::jxx::lang::jint columnNumber_;
};

} // namespace jxx::org::xml::sax
