#pragma once
#include "ext/xml/parsers/jxx.ext.xml.parsers.DocumentBuilder.h"
namespace jxx::ext::xml::parsers::internal
{
	class PugiDocumentBuilder final : public jxx::lang::ClassBase<PugiDocumentBuilder, DocumentBuilder>
	{
	public: PugiDocumentBuilder(jxx::lang::jbool namespaceAware, jxx::lang::jbool validating, jxx::lang::jbool ignoreWhitespace, jxx::lang::jbool expandEntities, jxx::lang::jbool ignoreComments, jxx::lang::jbool coalescing); jxx::Ptr<jxx::org::w3c::dom::Document> parse(const jxx::Ptr<jxx::org::xml::sax::InputSource>& input) override; jxx::Ptr<jxx::org::w3c::dom::Document> newDocument() override; jxx::Ptr<jxx::org::w3c::dom::DOMImplementation> getDOMImplementation() override; void setEntityResolver(const jxx::Ptr<jxx::org::xml::sax::EntityResolver>& resolver) override; void setErrorHandler(const jxx::Ptr<jxx::org::xml::sax::ErrorHandler>& handler) override; jxx::lang::jbool isNamespaceAware()const override; jxx::lang::jbool isValidating()const override; void reset() override; private: jxx::lang::jbool namespaceAware_, validating_, ignoreWhitespace_, expandEntities_, ignoreComments_, coalescing_; jxx::Ptr<jxx::org::xml::sax::EntityResolver> resolver_; jxx::Ptr<jxx::org::xml::sax::ErrorHandler> handler_;
	};
}
