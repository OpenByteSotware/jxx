#include "org/xml/sax/internal/jxx.org.xml.sax.internal.PugiXMLReader.h"
#include <functional>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>
#include <pugixml.hpp>
#include "io/jxx.io.InputStream.h"
#include "io/jxx.io.Reader.h"
#include "lang/jxx.lang.NullPointerException.h"
#include "lang/jxx.lang.String.h"
#include "org/xml/sax/helpers/jxx.org.xml.sax.helpers.AttributesImpl.h"
#include "org/xml/sax/helpers/jxx.org.xml.sax.helpers.LocatorImpl.h"
#include "org/xml/sax/jxx.org.xml.sax.ContentHandler.h"
#include "org/xml/sax/jxx.org.xml.sax.DTDHandler.h"
#include "org/xml/sax/jxx.org.xml.sax.EntityResolver.h"
#include "org/xml/sax/jxx.org.xml.sax.ErrorHandler.h"
#include "org/xml/sax/jxx.org.xml.sax.InputSource.h"
#include "org/xml/sax/jxx.org.xml.sax.SAXNotRecognizedException.h"
#include "org/xml/sax/jxx.org.xml.sax.SAXNotSupportedException.h"
#include "org/xml/sax/jxx.org.xml.sax.SAXParseException.h"
namespace jxx::org::xml::sax::internal {
namespace {
const char* NAMESPACES="http://xml.org/sax/features/namespaces";
const char* PREFIXES="http://xml.org/sax/features/namespace-prefixes";
const char* VALIDATION="http://xml.org/sax/features/validation";
std::string text(const ::jxx::Ptr<::jxx::lang::String>& s){return s?s->utf8():std::string();}
std::string prefix(const std::string& q){auto p=q.find(':');return p==std::string::npos?std::string():q.substr(0,p);}
std::string local(const std::string& q){auto p=q.find(':');return p==std::string::npos?q:q.substr(p+1);}
std::string ns(pugi::xml_node n,const std::string& q){auto p=prefix(q);auto d=p.empty()?std::string("xmlns"):std::string("xmlns:")+p;for(auto x=n;x;x=x.parent()){auto a=x.attribute(d.c_str());if(a)return a.value();}return {};}
std::string readChars(const ::jxx::Ptr<::jxx::io::Reader>& in){std::u16string v;for(;;){auto c=in->read();if(c<0)break;v.push_back(static_cast<char16_t>(c));}return ::jxx::NEW<::jxx::lang::String>(v)->utf8();}
std::string readBytes(const ::jxx::Ptr<::jxx::io::InputStream>& in){std::string v;for(;;){auto c=in->read();if(c<0)break;v.push_back(static_cast<char>(c));}return v;}
::jxx::lang::CharArray chars(const char* value){return ::jxx::NEW<::jxx::lang::String>(value?value:"")->toCharArray();}
}
struct PugiXMLReader::State {::jxx::Ptr<::jxx::org::xml::sax::EntityResolver> entity;::jxx::Ptr<::jxx::org::xml::sax::DTDHandler> dtd;::jxx::Ptr<::jxx::org::xml::sax::ContentHandler> content;::jxx::Ptr<::jxx::org::xml::sax::ErrorHandler> error;bool namespaces=true;bool prefixes=false;bool parsing=false;};
PugiXMLReader::PugiXMLReader():Super(),state_(std::make_unique<State>()){} PugiXMLReader::~PugiXMLReader()=default;
::jxx::lang::jbool PugiXMLReader::getFeature(const ::jxx::Ptr<::jxx::lang::String>& name)const{if(!name)throw ::jxx::lang::NullPointerException();auto n=name->utf8();if(n==NAMESPACES)return state_->namespaces;if(n==PREFIXES)return state_->prefixes;if(n==VALIDATION)return false;throw ::jxx::org::xml::sax::SAXNotRecognizedException(name);}
void PugiXMLReader::setFeature(const ::jxx::Ptr<::jxx::lang::String>& name,::jxx::lang::jbool value){if(!name)throw ::jxx::lang::NullPointerException();if(state_->parsing)throw ::jxx::org::xml::sax::SAXNotSupportedException(name);auto n=name->utf8();if(n==NAMESPACES){state_->namespaces=value;return;}if(n==PREFIXES){state_->prefixes=value;return;}if(n==VALIDATION){if(value)throw ::jxx::org::xml::sax::SAXNotSupportedException(name);return;}throw ::jxx::org::xml::sax::SAXNotRecognizedException(name);}
::jxx::Ptr<::jxx::lang::Object> PugiXMLReader::getProperty(const ::jxx::Ptr<::jxx::lang::String>& name)const{if(!name)throw ::jxx::lang::NullPointerException();throw ::jxx::org::xml::sax::SAXNotRecognizedException(name);}
void PugiXMLReader::setProperty(const ::jxx::Ptr<::jxx::lang::String>& name,const ::jxx::Ptr<::jxx::lang::Object>& value){(void)value;if(!name)throw ::jxx::lang::NullPointerException();throw ::jxx::org::xml::sax::SAXNotRecognizedException(name);}
void PugiXMLReader::setEntityResolver(const ::jxx::Ptr<::jxx::org::xml::sax::EntityResolver>& v){state_->entity=v;} auto PugiXMLReader::getEntityResolver()const->::jxx::Ptr<::jxx::org::xml::sax::EntityResolver>{return state_->entity;}
void PugiXMLReader::setDTDHandler(const ::jxx::Ptr<::jxx::org::xml::sax::DTDHandler>& v){state_->dtd=v;} auto PugiXMLReader::getDTDHandler()const->::jxx::Ptr<::jxx::org::xml::sax::DTDHandler>{return state_->dtd;}
void PugiXMLReader::setContentHandler(const ::jxx::Ptr<::jxx::org::xml::sax::ContentHandler>& v){state_->content=v;} auto PugiXMLReader::getContentHandler()const->::jxx::Ptr<::jxx::org::xml::sax::ContentHandler>{return state_->content;}
void PugiXMLReader::setErrorHandler(const ::jxx::Ptr<::jxx::org::xml::sax::ErrorHandler>& v){state_->error=v;} auto PugiXMLReader::getErrorHandler()const->::jxx::Ptr<::jxx::org::xml::sax::ErrorHandler>{return state_->error;}
void PugiXMLReader::parse(const ::jxx::Ptr<::jxx::lang::String>& systemId){if(!systemId)throw ::jxx::lang::NullPointerException();parse(::jxx::NEW<::jxx::org::xml::sax::InputSource>(systemId));}
void PugiXMLReader::parse(const ::jxx::Ptr<::jxx::org::xml::sax::InputSource>& input){
 if(!input)throw ::jxx::lang::NullPointerException();if(state_->parsing)throw ::jxx::org::xml::sax::SAXNotSupportedException(::jxx::NEW<::jxx::lang::String>("Reader is already parsing"));
 struct Guard{bool& v;explicit Guard(bool& x):v(x){v=true;}~Guard(){v=false;}} guard(state_->parsing);
 pugi::xml_document doc;pugi::xml_parse_result result;auto cr=input->getCharacterStream();auto br=input->getByteStream();std::string data;
 if(cr){data=readChars(cr);result=doc.load_buffer(data.data(),data.size(),pugi::parse_default|pugi::parse_comments|pugi::parse_pi|pugi::parse_cdata);}
 else if(br){data=readBytes(br);result=doc.load_buffer(data.data(),data.size(),pugi::parse_default|pugi::parse_comments|pugi::parse_pi|pugi::parse_cdata);}
 else if(input->getSystemId()){result=doc.load_file(input->getSystemId()->utf8().c_str(),pugi::parse_default|pugi::parse_comments|pugi::parse_pi|pugi::parse_cdata);}
 else throw ::jxx::lang::NullPointerException();
 if(!result){auto ex=::jxx::NEW<::jxx::org::xml::sax::SAXParseException>(::jxx::NEW<::jxx::lang::String>(result.description()),input->getPublicId(),input->getSystemId(),1,static_cast<::jxx::lang::jint>(result.offset+1));if(state_->error)state_->error->fatalError(ex);throw *ex;}
 auto handler=state_->content;if(!handler)return;auto locator=::jxx::NEW<::jxx::org::xml::sax::helpers::LocatorImpl>();locator->setPublicId(input->getPublicId());locator->setSystemId(input->getSystemId());locator->setLineNumber(1);locator->setColumnNumber(1);handler->setDocumentLocator(locator);handler->startDocument();
 std::function<void(pugi::xml_node)> visit=[&](pugi::xml_node node){
  if(node.type()==pugi::node_element){std::vector<std::string> mappings;for(auto a:node.attributes()){std::string q=a.name();if(q=="xmlns"||q.rfind("xmlns:",0)==0){auto p=q=="xmlns"?std::string():q.substr(6);handler->startPrefixMapping(::jxx::NEW<::jxx::lang::String>(p),::jxx::NEW<::jxx::lang::String>(a.value()));mappings.push_back(p);}}
   auto attrs=::jxx::NEW<::jxx::org::xml::sax::helpers::AttributesImpl>();for(auto a:node.attributes()){std::string q=a.name();bool decl=q=="xmlns"||q.rfind("xmlns:",0)==0;if(decl&&!state_->prefixes)continue;auto u=decl?std::string("http://www.w3.org/2000/xmlns/"):(state_->namespaces?ns(node,q):std::string());attrs->addAttribute(::jxx::NEW<::jxx::lang::String>(u),::jxx::NEW<::jxx::lang::String>(state_->namespaces?local(q):std::string()),::jxx::NEW<::jxx::lang::String>(state_->namespaces&& !state_->prefixes?std::string():q),::jxx::NEW<::jxx::lang::String>("CDATA"),::jxx::NEW<::jxx::lang::String>(a.value()));}
   std::string q=node.name(),u=state_->namespaces?ns(node,q):std::string(),l=state_->namespaces?local(q):std::string(),qn=state_->namespaces&&!state_->prefixes?std::string():q;handler->startElement(::jxx::NEW<::jxx::lang::String>(u),::jxx::NEW<::jxx::lang::String>(l),::jxx::NEW<::jxx::lang::String>(qn),attrs);for(auto c:node.children())visit(c);handler->endElement(::jxx::NEW<::jxx::lang::String>(u),::jxx::NEW<::jxx::lang::String>(l),::jxx::NEW<::jxx::lang::String>(qn));for(auto i=mappings.rbegin();i!=mappings.rend();++i)handler->endPrefixMapping(::jxx::NEW<::jxx::lang::String>(*i));return;}
  if(node.type()==pugi::node_pcdata||node.type()==pugi::node_cdata){auto a=chars(node.value());handler->characters(a,0,a->length);return;}if(node.type()==pugi::node_pi)handler->processingInstruction(::jxx::NEW<::jxx::lang::String>(node.name()),::jxx::NEW<::jxx::lang::String>(node.value()));};
 for(auto node:doc.children())visit(node);handler->endDocument();
}
} // namespace jxx::org::xml::sax::internal
