#pragma once
#include "ext/xml/transform/jxx.ext.xml.transform.Source.h"
#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Object.h"
#include "org/w3c/dom/jxx.org.w3c.dom.Node.h"
namespace jxx::ext::xml::transform::dom {
class DOMSource final : public ::jxx::lang::ClassBase<DOMSource, ::jxx::lang::Object, ::jxx::ext::xml::transform::Source> {
public:
    DOMSource();
    explicit DOMSource(const ::jxx::Ptr<::jxx::org::w3c::dom::Node>& node);
    DOMSource(const ::jxx::Ptr<::jxx::org::w3c::dom::Node>& node, const ::jxx::Ptr<::jxx::lang::String>& systemId);
    void setNode(const ::jxx::Ptr<::jxx::org::w3c::dom::Node>& node);
    ::jxx::Ptr<::jxx::org::w3c::dom::Node> getNode() const;
    void setSystemId(const ::jxx::Ptr<::jxx::lang::String>& systemId) override;
    ::jxx::Ptr<::jxx::lang::String> getSystemId() const override;
    static ::jxx::Ptr<::jxx::lang::String> FEATURE();
private:
    ::jxx::Ptr<::jxx::org::w3c::dom::Node> node_;
    ::jxx::Ptr<::jxx::lang::String> systemId_;
};
}
