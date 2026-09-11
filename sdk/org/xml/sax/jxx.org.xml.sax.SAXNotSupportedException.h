#pragma once

#include "lang/jxx.lang.ClassInfoMarker.h"
#include "org/xml/sax/jxx.org.xml.sax.SAXException.h"

namespace jxx::lang {
class ClassAny;
class Object;
class String;
}

namespace jxx::org::xml::sax {

class SAXNotSupportedException
    : public SAXException {
public:
    using JxxSuper = SAXException;
    using JxxClassInfoMarker =
        ::jxx::lang::ClassInfo<SAXNotSupportedException, JxxSuper>;

    static ::jxx::Ptr<::jxx::lang::ClassAny> Class();

    SAXNotSupportedException();

    explicit SAXNotSupportedException(
        const ::jxx::Ptr<::jxx::lang::String>& message);

    SAXNotSupportedException(
        const SAXNotSupportedException& other);

    SAXNotSupportedException(
        SAXNotSupportedException&& other) noexcept;

    SAXNotSupportedException& operator=(
        const SAXNotSupportedException& other);

    SAXNotSupportedException& operator=(
        SAXNotSupportedException&& other) noexcept;

    ~SAXNotSupportedException() override;

protected:
    ::jxx::Ptr<::jxx::lang::Object>
    cloneImpl() const override;

    const char* typeName() const noexcept override;
};

} // namespace jxx::org::xml::sax
