#include "org/xml/sax/jxx.org.xml.sax.SAXException.h"
#include "lang/jxx.lang.String.h"
namespace jxx::org::xml::sax { SAXException::SAXException()=default; SAXException::SAXException(const jxx::Ptr<jxx::lang::String>& m):jxx::lang::Exception(m){} SAXException::SAXException(const jxx::Ptr<jxx::lang::String>& m,const jxx::Ptr<jxx::lang::Throwable>& c):jxx::lang::Exception(m,c){} const char* SAXException::typeName() const noexcept{return "SAXException";} jxx::Ptr<jxx::lang::Object> SAXException::cloneImpl() const{return jxx::CAST<jxx::lang::Object>(jxx::NEW<SAXException>(*this));} }
