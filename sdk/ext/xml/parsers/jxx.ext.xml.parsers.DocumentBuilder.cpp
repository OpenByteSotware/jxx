#include "ext/xml/parsers/jxx.ext.xml.parsers.DocumentBuilder.h"

#include "io/jxx.io.File.h"
#include "lang/jxx.lang.IllegalArgumentException.h"
#include "lang/jxx.lang.String.h"
#include "org/xml/sax/jxx.org.xml.sax.InputSource.h"

namespace jxx::ext::xml::parsers {

DocumentBuilder::DocumentBuilder() = default;
DocumentBuilder::~DocumentBuilder() = default;

::jxx::Ptr<::jxx::org::w3c::dom::Document> DocumentBuilder::parse(
    const ::jxx::Ptr<::jxx::io::InputStream>& input) {
    if (input == nullptr) {
        throw ::jxx::lang::IllegalArgumentException(
            ::jxx::NEW<::jxx::lang::String>("input"));
    }
    return parse(::jxx::NEW<::jxx::org::xml::sax::InputSource>(input));
}

::jxx::Ptr<::jxx::org::w3c::dom::Document> DocumentBuilder::parse(
    const ::jxx::Ptr<::jxx::io::InputStream>& input,
    const ::jxx::Ptr<::jxx::lang::String>& systemId) {
    if (input == nullptr) {
        throw ::jxx::lang::IllegalArgumentException(
            ::jxx::NEW<::jxx::lang::String>("input"));
    }
    auto source = ::jxx::NEW<::jxx::org::xml::sax::InputSource>(input);
    source->setSystemId(systemId);
    return parse(source);
}

::jxx::Ptr<::jxx::org::w3c::dom::Document> DocumentBuilder::parse(
    const ::jxx::Ptr<::jxx::io::File>& file) {
    if (file == nullptr) {
        throw ::jxx::lang::IllegalArgumentException(
            ::jxx::NEW<::jxx::lang::String>("file"));
    }
    return parse(file->getPath());
}

::jxx::Ptr<::jxx::org::w3c::dom::Document> DocumentBuilder::parse(
    const ::jxx::Ptr<::jxx::lang::String>& uri) {
    if (uri == nullptr) {
        throw ::jxx::lang::IllegalArgumentException(
            ::jxx::NEW<::jxx::lang::String>("uri"));
    }
    return parse(::jxx::NEW<::jxx::org::xml::sax::InputSource>(uri));
}

void DocumentBuilder::reset() {
}

} // namespace jxx::ext::xml::parsers
