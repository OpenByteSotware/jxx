#include "ext/xml/parsers/jxx.ext.xml.parsers.SAXParserFactory.h"
#include "ext/xml/parsers/internal/jxx.ext.xml.parsers.internal.PugiSAXParserFactory.h"
#include "ext/xml/parsers/jxx.ext.xml.parsers.FactoryConfigurationError.h"
#include "lang/jxx.lang.ClassLoader.h"
#include "lang/jxx.lang.String.h"
namespace jxx::ext::xml::parsers {
SAXParserFactory::SAXParserFactory():Super(){} SAXParserFactory::~SAXParserFactory()=default;
::jxx::Ptr<SAXParserFactory> SAXParserFactory::newInstance(){return ::jxx::NEW<::jxx::ext::xml::parsers::internal::PugiSAXParserFactory>();}
::jxx::Ptr<SAXParserFactory> SAXParserFactory::newInstance(const ::jxx::Ptr<::jxx::lang::String>& name,const ::jxx::Ptr<::jxx::lang::ClassLoader>& loader){(void)loader;if(name==nullptr||name->length()==0)return newInstance();throw FactoryConfigurationError(::jxx::NEW<::jxx::lang::String>("Requested SAX parser factory is unavailable"));}
void SAXParserFactory::setNamespaceAware(::jxx::lang::jbool v){namespaceAware_=v;}::jxx::lang::jbool SAXParserFactory::isNamespaceAware()const{return namespaceAware_;}
void SAXParserFactory::setValidating(::jxx::lang::jbool v){validating_=v;}::jxx::lang::jbool SAXParserFactory::isValidating()const{return validating_;}
void SAXParserFactory::setSchema(const ::jxx::Ptr<::jxx::ext::xml::validation::Schema>& v){schema_=v;}auto SAXParserFactory::getSchema()const->::jxx::Ptr<::jxx::ext::xml::validation::Schema>{return schema_;}
void SAXParserFactory::setXIncludeAware(::jxx::lang::jbool v){xincludeAware_=v;}::jxx::lang::jbool SAXParserFactory::isXIncludeAware()const{return xincludeAware_;}
} // namespace jxx::ext::xml::parsers
