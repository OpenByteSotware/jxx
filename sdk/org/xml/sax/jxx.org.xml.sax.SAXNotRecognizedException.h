#pragma once

#include "lang/jxx.lang.ClassInfoMarker.h"
#include "org/xml/sax/jxx.org.xml.sax.SAXException.h"

namespace jxx::lang {
class ClassAny;
class Object;
class String;
}

namespace jxx::org::xml::sax {

class SAXNotRecognizedException
    : public SAXException {
public:
    using JxxSuper = SAXException;
    using JxxClassInfoMarker =
        ::jxx::lang::ClassInfo<SAXNotRecognizedException, JxxSuper>;

    static ::jxx::Ptr<::jxx::lang::ClassAny> Class();

    SAXNotRecognizedException();

    explicit SAXNotRecognizedException(
        const ::jxx::Ptr<::jxx::lang::String>& message);

    SAXNotRecognizedException(
        const SAXNotRecognizedException& other);

    SAXNotRecognizedException(
        SAXNotRecognizedException&& other) noexcept;

    SAXNotRecognizedException& operator=(
        const SAXNotRecognizedException& other);

    SAXNotRecognizedException& operator=(
        SAXNotRecognizedException&& other) noexcept;

    ~SAXNotRecognizedException() override;

protected:
    ::jxx::Ptr<::jxx::lang::Object>
    cloneImpl() const override;

    const char* typeName() const noexcept override;
};

} // namespace jxx::org::xml::sax
