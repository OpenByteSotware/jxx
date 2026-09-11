#include "org/xml/sax/helpers/jxx.org.xml.sax.helpers.XMLFilterImpl.h"

#include "lang/jxx.lang.NullPointerException.h"
#include "lang/jxx.lang.String.h"
#include "org/xml/sax/jxx.org.xml.sax.InputSource.h"
#include "org/xml/sax/jxx.org.xml.sax.SAXException.h"
#include "org/xml/sax/jxx.org.xml.sax.SAXParseException.h"

namespace jxx::org::xml::sax::helpers {

XMLFilterImpl::XMLFilterImpl() : Super() {}
XMLFilterImpl::XMLFilterImpl(const ::jxx::Ptr<::jxx::org::xml::sax::XMLReader>& parent)
    : Super(), parent_(parent) {
    if (parent == nullptr) throw ::jxx::lang::NullPointerException();
}
XMLFilterImpl::~XMLFilterImpl() = default;

void XMLFilterImpl::setParent(const ::jxx::Ptr<::jxx::org::xml::sax::XMLReader>& parent) {
    if (parent == nullptr) throw ::jxx::lang::NullPointerException();
    parent_ = parent;
}
auto XMLFilterImpl::getParent() const -> ::jxx::Ptr<::jxx::org::xml::sax::XMLReader> { return parent_; }

::jxx::lang::jbool XMLFilterImpl::getFeature(const ::jxx::Ptr<::jxx::lang::String>& name) const {
    if (parent_ == nullptr) throw ::jxx::org::xml::sax::SAXException(::jxx::NEW<::jxx::lang::String>("No parent for filter"));
    return parent_->getFeature(name);
}
void XMLFilterImpl::setFeature(const ::jxx::Ptr<::jxx::lang::String>& name, ::jxx::lang::jbool value) {
    if (parent_ == nullptr) throw ::jxx::org::xml::sax::SAXException(::jxx::NEW<::jxx::lang::String>("No parent for filter"));
    parent_->setFeature(name, value);
}
auto XMLFilterImpl::getProperty(const ::jxx::Ptr<::jxx::lang::String>& name) const -> ::jxx::Ptr<::jxx::lang::Object> {
    if (parent_ == nullptr) throw ::jxx::org::xml::sax::SAXException(::jxx::NEW<::jxx::lang::String>("No parent for filter"));
    return parent_->getProperty(name);
}
void XMLFilterImpl::setProperty(const ::jxx::Ptr<::jxx::lang::String>& name, const ::jxx::Ptr<::jxx::lang::Object>& value) {
    if (parent_ == nullptr) throw ::jxx::org::xml::sax::SAXException(::jxx::NEW<::jxx::lang::String>("No parent for filter"));
    parent_->setProperty(name, value);
}
void XMLFilterImpl::setEntityResolver(const ::jxx::Ptr<::jxx::org::xml::sax::EntityResolver>& v){entityResolver_=v;} auto XMLFilterImpl::getEntityResolver()const->::jxx::Ptr<::jxx::org::xml::sax::EntityResolver>{return entityResolver_;}
void XMLFilterImpl::setDTDHandler(const ::jxx::Ptr<::jxx::org::xml::sax::DTDHandler>& v){dtdHandler_=v;} auto XMLFilterImpl::getDTDHandler()const->::jxx::Ptr<::jxx::org::xml::sax::DTDHandler>{return dtdHandler_;}
void XMLFilterImpl::setContentHandler(const ::jxx::Ptr<::jxx::org::xml::sax::ContentHandler>& v){contentHandler_=v;} auto XMLFilterImpl::getContentHandler()const->::jxx::Ptr<::jxx::org::xml::sax::ContentHandler>{return contentHandler_;}
void XMLFilterImpl::setErrorHandler(const ::jxx::Ptr<::jxx::org::xml::sax::ErrorHandler>& v){errorHandler_=v;} auto XMLFilterImpl::getErrorHandler()const->::jxx::Ptr<::jxx::org::xml::sax::ErrorHandler>{return errorHandler_;}

void XMLFilterImpl::setupParse_(){
    if(parent_==nullptr)throw ::jxx::org::xml::sax::SAXException(::jxx::NEW<::jxx::lang::String>("No parent for filter"));
    const auto self=thisPtr();
    parent_->setEntityResolver(::jxx::CAST<::jxx::org::xml::sax::EntityResolver>(self));
    parent_->setDTDHandler(::jxx::CAST<::jxx::org::xml::sax::DTDHandler>(self));
    parent_->setContentHandler(::jxx::CAST<::jxx::org::xml::sax::ContentHandler>(self));
    parent_->setErrorHandler(::jxx::CAST<::jxx::org::xml::sax::ErrorHandler>(self));
}
void XMLFilterImpl::parse(const ::jxx::Ptr<::jxx::org::xml::sax::InputSource>& input){setupParse_();parent_->parse(input);}
void XMLFilterImpl::parse(const ::jxx::Ptr<::jxx::lang::String>& systemId){setupParse_();parent_->parse(systemId);}

auto XMLFilterImpl::resolveEntity(const ::jxx::Ptr<::jxx::lang::String>& p,const ::jxx::Ptr<::jxx::lang::String>& s)->::jxx::Ptr<::jxx::org::xml::sax::InputSource>{return entityResolver_?entityResolver_->resolveEntity(p,s):nullptr;}
void XMLFilterImpl::notationDecl(const ::jxx::Ptr<::jxx::lang::String>& n,const ::jxx::Ptr<::jxx::lang::String>& p,const ::jxx::Ptr<::jxx::lang::String>& s){if(dtdHandler_)dtdHandler_->notationDecl(n,p,s);}
void XMLFilterImpl::unparsedEntityDecl(const ::jxx::Ptr<::jxx::lang::String>& n,const ::jxx::Ptr<::jxx::lang::String>& p,const ::jxx::Ptr<::jxx::lang::String>& s,const ::jxx::Ptr<::jxx::lang::String>& notation){if(dtdHandler_)dtdHandler_->unparsedEntityDecl(n,p,s,notation);}
void XMLFilterImpl::setDocumentLocator(const ::jxx::Ptr<::jxx::org::xml::sax::Locator>& v){if(contentHandler_)contentHandler_->setDocumentLocator(v);} void XMLFilterImpl::startDocument(){if(contentHandler_)contentHandler_->startDocument();} void XMLFilterImpl::endDocument(){if(contentHandler_)contentHandler_->endDocument();}
void XMLFilterImpl::startPrefixMapping(const ::jxx::Ptr<::jxx::lang::String>& p,const ::jxx::Ptr<::jxx::lang::String>& u){if(contentHandler_)contentHandler_->startPrefixMapping(p,u);} void XMLFilterImpl::endPrefixMapping(const ::jxx::Ptr<::jxx::lang::String>& p){if(contentHandler_)contentHandler_->endPrefixMapping(p);}
void XMLFilterImpl::startElement(const ::jxx::Ptr<::jxx::lang::String>& u,const ::jxx::Ptr<::jxx::lang::String>& l,const ::jxx::Ptr<::jxx::lang::String>& q,const ::jxx::Ptr<::jxx::org::xml::sax::Attributes>& a){if(contentHandler_)contentHandler_->startElement(u,l,q,a);} void XMLFilterImpl::endElement(const ::jxx::Ptr<::jxx::lang::String>& u,const ::jxx::Ptr<::jxx::lang::String>& l,const ::jxx::Ptr<::jxx::lang::String>& q){if(contentHandler_)contentHandler_->endElement(u,l,q);}
void XMLFilterImpl::characters(const ::jxx::lang::CharArray& c,::jxx::lang::jint s,::jxx::lang::jint l){if(contentHandler_)contentHandler_->characters(c,s,l);} void XMLFilterImpl::ignorableWhitespace(const ::jxx::lang::CharArray& c,::jxx::lang::jint s,::jxx::lang::jint l){if(contentHandler_)contentHandler_->ignorableWhitespace(c,s,l);} void XMLFilterImpl::processingInstruction(const ::jxx::Ptr<::jxx::lang::String>& t,const ::jxx::Ptr<::jxx::lang::String>& d){if(contentHandler_)contentHandler_->processingInstruction(t,d);} void XMLFilterImpl::skippedEntity(const ::jxx::Ptr<::jxx::lang::String>& n){if(contentHandler_)contentHandler_->skippedEntity(n);}
void XMLFilterImpl::warning(const ::jxx::Ptr<::jxx::org::xml::sax::SAXParseException>& e){if(errorHandler_)errorHandler_->warning(e);} void XMLFilterImpl::error(const ::jxx::Ptr<::jxx::org::xml::sax::SAXParseException>& e){if(errorHandler_)errorHandler_->error(e);} void XMLFilterImpl::fatalError(const ::jxx::Ptr<::jxx::org::xml::sax::SAXParseException>& e){if(errorHandler_)errorHandler_->fatalError(e);}

} // namespace jxx::org::xml::sax::helpers
