#pragma once

#include "ext/xml/parsers/jxx.ext.xml.parsers.DocumentBuilder.h"

namespace jxx::ext::xml::validation {
class Schema;
}

namespace jxx::ext::xml::parsers::internal {

class PugiDocumentBuilder final
    : public ::jxx::lang::ClassBase<
          PugiDocumentBuilder,
          DocumentBuilder> {
public:
    using JxxSuper = DocumentBuilder;
    using Super = ::jxx::lang::ClassBase<PugiDocumentBuilder, JxxSuper>;

    PugiDocumentBuilder(
        ::jxx::lang::jbool namespaceAware,
        ::jxx::lang::jbool validating,
        ::jxx::lang::jbool ignoreWhitespace,
        ::jxx::lang::jbool expandEntities,
        ::jxx::lang::jbool ignoreComments,
        ::jxx::lang::jbool coalescing,
        ::jxx::lang::jbool xincludeAware,
        const ::jxx::Ptr<::jxx::ext::xml::validation::Schema>& schema);

    ~PugiDocumentBuilder() override;

    ::jxx::Ptr<::jxx::org::w3c::dom::Document> parse(
        const ::jxx::Ptr<::jxx::org::xml::sax::InputSource>& input) override;
    ::jxx::Ptr<::jxx::org::w3c::dom::Document> newDocument() override;
    ::jxx::Ptr<::jxx::org::w3c::dom::DOMImplementation> getDOMImplementation() override;
    void setEntityResolver(const ::jxx::Ptr<::jxx::org::xml::sax::EntityResolver>& resolver) override;
    void setErrorHandler(const ::jxx::Ptr<::jxx::org::xml::sax::ErrorHandler>& handler) override;
    ::jxx::lang::jbool isNamespaceAware() const override;
    ::jxx::lang::jbool isValidating() const override;
    ::jxx::Ptr<::jxx::ext::xml::validation::Schema> getSchema() const override;
    ::jxx::lang::jbool isXIncludeAware() const override;
    void reset() override;

private:
    ::jxx::lang::jbool namespaceAware_;
    ::jxx::lang::jbool validating_;
    ::jxx::lang::jbool ignoreWhitespace_;
    ::jxx::lang::jbool expandEntities_;
    ::jxx::lang::jbool ignoreComments_;
    ::jxx::lang::jbool coalescing_;
    ::jxx::lang::jbool xincludeAware_;
    ::jxx::Ptr<::jxx::ext::xml::validation::Schema> schema_;
    ::jxx::Ptr<::jxx::org::xml::sax::EntityResolver> resolver_;
    ::jxx::Ptr<::jxx::org::xml::sax::ErrorHandler> handler_;
};

} // namespace jxx::ext::xml::parsers::internal
