#include "ext/xml/parsers/internal/jxx.ext.xml.parsers.internal.PugiDocumentBuilderFactory.h"

#include "ext/xml/parsers/internal/jxx.ext.xml.parsers.internal.PugiDocumentBuilder.h"
#include "ext/xml/parsers/jxx.ext.xml.parsers.ParserConfigurationException.h"
#include "lang/jxx.lang.IllegalArgumentException.h"
#include "lang/jxx.lang.NullPointerException.h"
#include "lang/jxx.lang.String.h"

namespace jxx::ext::xml::parsers::internal {

PugiDocumentBuilderFactory::PugiDocumentBuilderFactory() = default;
PugiDocumentBuilderFactory::~PugiDocumentBuilderFactory() = default;

::jxx::Ptr<DocumentBuilder> PugiDocumentBuilderFactory::newDocumentBuilder() {
    if (validating_ || xinclude_ || schema_ != nullptr) {
        throw ParserConfigurationException(
            ::jxx::NEW<::jxx::lang::String>(
                "Validation, schema, and XInclude processing are not supported"));
    }
    return ::jxx::NEW<PugiDocumentBuilder>(
        namespaceAware_, validating_, ignoreWhitespace_, expandEntities_,
        ignoreComments_, coalescing_, xinclude_, schema_);
}

void PugiDocumentBuilderFactory::setAttribute(
    const ::jxx::Ptr<::jxx::lang::String>& name,
    const ::jxx::Ptr<::jxx::lang::Object>& value) {
    if (name == nullptr) throw ::jxx::lang::NullPointerException();
    attributes_[name->utf8()] = value;
}

::jxx::Ptr<::jxx::lang::Object> PugiDocumentBuilderFactory::getAttribute(
    const ::jxx::Ptr<::jxx::lang::String>& name) {
    if (name == nullptr) throw ::jxx::lang::NullPointerException();
    const auto found = attributes_.find(name->utf8());
    if (found == attributes_.end()) {
        throw ::jxx::lang::IllegalArgumentException(
            ::jxx::NEW<::jxx::lang::String>("Unknown attribute"));
    }
    return found->second;
}

void PugiDocumentBuilderFactory::setFeature(
    const ::jxx::Ptr<::jxx::lang::String>& name,
    ::jxx::lang::jbool value) {
    if (name == nullptr) throw ::jxx::lang::NullPointerException();
    features_[name->utf8()] = value;
}

::jxx::lang::jbool PugiDocumentBuilderFactory::getFeature(
    const ::jxx::Ptr<::jxx::lang::String>& name) {
    if (name == nullptr) throw ::jxx::lang::NullPointerException();
    const auto found = features_.find(name->utf8());
    if (found == features_.end()) {
        throw ParserConfigurationException(
            ::jxx::NEW<::jxx::lang::String>("Unknown feature"));
    }
    return found->second;
}

} // namespace jxx::ext::xml::parsers::internal
