#include "ext/xml/transform/internal/jxx.ext.xml.transform.internal.IdentityTransformerFactory.h"
#include "ext/xml/transform/dom/jxx.ext.xml.transform.dom.DOMSource.h"
#include "ext/xml/transform/internal/jxx.ext.xml.transform.internal.IdentityTransformer.h"
#include "ext/xml/transform/jxx.ext.xml.transform.TransformerConfigurationException.h"
#include "ext/xml/transform/stream/jxx.ext.xml.transform.stream.StreamResult.h"
#include "lang/jxx.lang.IllegalArgumentException.h"
#include "lang/jxx.lang.NullPointerException.h"
namespace jxx::ext::xml::transform::internal {
::jxx::Ptr<::jxx::ext::xml::transform::Transformer> IdentityTransformerFactory::newTransformer(){return ::jxx::NEW<IdentityTransformer>();}
::jxx::Ptr<::jxx::ext::xml::transform::Transformer> IdentityTransformerFactory::newTransformer(const ::jxx::Ptr<::jxx::ext::xml::transform::Source>& source){if(source==nullptr)throw ::jxx::ext::xml::transform::TransformerConfigurationException(::jxx::NEW<::jxx::lang::String>("source is null"));throw ::jxx::ext::xml::transform::TransformerConfigurationException(::jxx::NEW<::jxx::lang::String>("stylesheet transformations are not supported by the identity-transform core"));}
void IdentityTransformerFactory::setAttribute(const ::jxx::Ptr<::jxx::lang::String>& name,const ::jxx::Ptr<::jxx::lang::Object>& value){if(name==nullptr)throw ::jxx::lang::NullPointerException();attributes_[name->utf8()]=value;}
::jxx::Ptr<::jxx::lang::Object> IdentityTransformerFactory::getAttribute(const ::jxx::Ptr<::jxx::lang::String>& name)const{if(name==nullptr)throw ::jxx::lang::NullPointerException();const auto i=attributes_.find(name->utf8());if(i==attributes_.end())throw ::jxx::lang::IllegalArgumentException();return i->second;}
void IdentityTransformerFactory::setFeature(const ::jxx::Ptr<::jxx::lang::String>& name,::jxx::lang::jbool value){if(name==nullptr)throw ::jxx::lang::NullPointerException();if(!value||!getFeature(name))throw ::jxx::ext::xml::transform::TransformerConfigurationException(::jxx::NEW<::jxx::lang::String>("unsupported feature"));}
::jxx::lang::jbool IdentityTransformerFactory::getFeature(const ::jxx::Ptr<::jxx::lang::String>& name)const{if(name==nullptr)throw ::jxx::lang::NullPointerException();return name->equals(::jxx::ext::xml::transform::dom::DOMSource::FEATURE())||name->equals(::jxx::ext::xml::transform::stream::StreamResult::FEATURE());}
}
