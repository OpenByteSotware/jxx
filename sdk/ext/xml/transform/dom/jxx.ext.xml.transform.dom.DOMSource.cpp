#include "ext/xml/transform/dom/jxx.ext.xml.transform.dom.DOMSource.h"
namespace jxx::ext::xml::transform::dom {
DOMSource::DOMSource() = default;
DOMSource::DOMSource(const ::jxx::Ptr<::jxx::org::w3c::dom::Node>& node) : node_(node) {}
DOMSource::DOMSource(const ::jxx::Ptr<::jxx::org::w3c::dom::Node>& node,const ::jxx::Ptr<::jxx::lang::String>& systemId) : node_(node),systemId_(systemId) {}
void DOMSource::setNode(const ::jxx::Ptr<::jxx::org::w3c::dom::Node>& node){node_=node;}
::jxx::Ptr<::jxx::org::w3c::dom::Node> DOMSource::getNode() const{return node_;}
void DOMSource::setSystemId(const ::jxx::Ptr<::jxx::lang::String>& systemId){systemId_=systemId;}
::jxx::Ptr<::jxx::lang::String> DOMSource::getSystemId() const{return systemId_;}
::jxx::Ptr<::jxx::lang::String> DOMSource::FEATURE(){static const auto v=::jxx::NEW<::jxx::lang::String>("http://javax.xml.transform.dom.DOMSource/feature");return v;}
}
