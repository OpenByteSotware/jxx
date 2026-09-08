#include <string>
#include "ext/xml/parsers/internal/jxx.ext.xml.parsers.internal.PugiDocumentBuilder.h"
#include "io/jxx.io.InputStream.h"
#include "lang/jxx.lang.IllegalArgumentException.h"
#include "lang/jxx.lang.String.h"
#include "org/w3c/dom/internal/jxx.org.w3c.dom.internal.PugiDom.h"
#include "org/xml/sax/jxx.org.xml.sax.EntityResolver.h"
#include "org/xml/sax/jxx.org.xml.sax.ErrorHandler.h"
#include "org/xml/sax/jxx.org.xml.sax.InputSource.h"
#include "org/xml/sax/jxx.org.xml.sax.SAXParseException.h"

namespace jxx::ext::xml::parsers::internal
{
	PugiDocumentBuilder::PugiDocumentBuilder(jxx::lang::jbool n, jxx::lang::jbool v, jxx::lang::jbool w, jxx::lang::jbool e, jxx::lang::jbool i, jxx::lang::jbool c) :namespaceAware_(n), validating_(v), ignoreWhitespace_(w), expandEntities_(e), ignoreComments_(i), coalescing_(c)
	{
	}
	jxx::Ptr<jxx::org::w3c::dom::Document> PugiDocumentBuilder::parse(const jxx::Ptr<jxx::org::xml::sax::InputSource>& input)
	{
		if (!input)throw jxx::lang::IllegalArgumentException(jxx::NEW<jxx::lang::String>("input")); std::string error; jxx::lang::jint offset = 0; jxx::Ptr<jxx::org::w3c::dom::Document> document; if (auto stream = input->getByteStream()) {
			std::string bytes; for (;;) {
				auto b = stream->read(); if (b < 0)break; bytes.push_back(static_cast<char>(b));
			}document = jxx::org::w3c::dom::internal::parseDocument(bytes, ignoreComments_, coalescing_, error, offset);
		}
		else if (auto id = input->getSystemId()) {
			document = jxx::org::w3c::dom::internal::loadDocument(id->utf8(), ignoreComments_, coalescing_, error, offset);
		}
		else {
			throw jxx::lang::IllegalArgumentException(jxx::NEW<jxx::lang::String>("InputSource has no input"));
		}if (!document) {
			auto ex = jxx::NEW<jxx::org::xml::sax::SAXParseException>(jxx::NEW<jxx::lang::String>(error), input->getPublicId(), input->getSystemId(), 1, offset + 1); if (handler_)handler_->fatalError(ex); throw* ex;
		}return document;
	}
	jxx::Ptr<jxx::org::w3c::dom::Document> PugiDocumentBuilder::newDocument()
	{
		return jxx::org::w3c::dom::internal::newDocument();
	}
	jxx::Ptr<jxx::org::w3c::dom::DOMImplementation> PugiDocumentBuilder::getDOMImplementation()
	{
		return jxx::org::w3c::dom::internal::implementation();
	}
	void PugiDocumentBuilder::setEntityResolver(const jxx::Ptr<jxx::org::xml::sax::EntityResolver>& r)
	{
		resolver_ = r;
	}
	void PugiDocumentBuilder::setErrorHandler(const jxx::Ptr<jxx::org::xml::sax::ErrorHandler>& h)
	{
		handler_ = h;
	}
	jxx::lang::jbool PugiDocumentBuilder::isNamespaceAware()const
	{
		return namespaceAware_;
	}
	jxx::lang::jbool PugiDocumentBuilder::isValidating()const
	{
		return validating_;
	}
	void PugiDocumentBuilder::reset()
	{
		resolver_.reset(); handler_.reset();
	}
}
