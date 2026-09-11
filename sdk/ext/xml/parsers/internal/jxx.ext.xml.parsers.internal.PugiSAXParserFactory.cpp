#include "ext/xml/parsers/internal/jxx.ext.xml.parsers.internal.PugiSAXParserFactory.h"
#include <string>
#include <unordered_map>
#include "ext/xml/parsers/internal/jxx.ext.xml.parsers.internal.PugiSAXParser.h"
#include "ext/xml/parsers/jxx.ext.xml.parsers.ParserConfigurationException.h"
#include "lang/jxx.lang.String.h"
#include "org/xml/sax/helpers/jxx.org.xml.sax.helpers.XMLReaderFactory.h"
#include "org/xml/sax/jxx.org.xml.sax.XMLReader.h"
namespace jxx::ext::xml::parsers::internal {
PugiSAXParserFactory::PugiSAXParserFactory():SAXParserFactory(){}PugiSAXParserFactory::~PugiSAXParserFactory()=default;
::jxx::Ptr<::jxx::ext::xml::parsers::SAXParser> PugiSAXParserFactory::newSAXParser(){if(isValidating())throw ::jxx::ext::xml::parsers::ParserConfigurationException(::jxx::NEW<::jxx::lang::String>("Validation is not available"));if(isXIncludeAware())throw ::jxx::ext::xml::parsers::ParserConfigurationException(::jxx::NEW<::jxx::lang::String>("XInclude is not available"));auto p=::jxx::NEW<PugiSAXParser>(isNamespaceAware(),isValidating(),getSchema(),isXIncludeAware());for(const auto& f:features_)p->getXMLReader()->setFeature(::jxx::NEW<::jxx::lang::String>(f.first),f.second);return p;}
void PugiSAXParserFactory::setFeature(const ::jxx::Ptr<::jxx::lang::String>& n,::jxx::lang::jbool v){auto reader=::jxx::org::xml::sax::helpers::XMLReaderFactory::createXMLReader();reader->setFeature(n,v);features_[n->utf8()]=v;}
::jxx::lang::jbool PugiSAXParserFactory::getFeature(const ::jxx::Ptr<::jxx::lang::String>& n)const{auto i=features_.find(n->utf8());if(i!=features_.end())return i->second;auto reader=::jxx::org::xml::sax::helpers::XMLReaderFactory::createXMLReader();return reader->getFeature(n);}
} // namespace jxx::ext::xml::parsers::internal
