#include "org/xml/sax/helpers/jxx.org.xml.sax.helpers.XMLReaderFactory.h"
#include "lang/jxx.lang.String.h"
#include "org/xml/sax/internal/jxx.org.xml.sax.internal.PugiXMLReader.h"
#include "org/xml/sax/jxx.org.xml.sax.SAXException.h"
namespace jxx::org::xml::sax::helpers {
::jxx::Ptr<::jxx::org::xml::sax::XMLReader> XMLReaderFactory::createXMLReader(){return ::jxx::NEW<::jxx::org::xml::sax::internal::PugiXMLReader>();}
::jxx::Ptr<::jxx::org::xml::sax::XMLReader> XMLReaderFactory::createXMLReader(const ::jxx::Ptr<::jxx::lang::String>& className){
 if(className==nullptr||className->length()==0)return createXMLReader();
 throw ::jxx::org::xml::sax::SAXException(::jxx::NEW<::jxx::lang::String>("Custom XMLReader providers are not supported"));
}
} // namespace jxx::org::xml::sax::helpers
