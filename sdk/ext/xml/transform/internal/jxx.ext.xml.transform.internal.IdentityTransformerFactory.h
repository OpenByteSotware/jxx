#pragma once
#include <map>
#include <string>
#include "ext/xml/transform/jxx.ext.xml.transform.TransformerFactory.h"
namespace jxx::ext::xml::transform::internal {
class IdentityTransformerFactory final : public ::jxx::lang::ClassBase<IdentityTransformerFactory,::jxx::ext::xml::transform::TransformerFactory> {
public:
    ::jxx::Ptr<::jxx::ext::xml::transform::Transformer> newTransformer() override;
    ::jxx::Ptr<::jxx::ext::xml::transform::Transformer> newTransformer(const ::jxx::Ptr<::jxx::ext::xml::transform::Source>& source) override;
    void setAttribute(const ::jxx::Ptr<::jxx::lang::String>& name,const ::jxx::Ptr<::jxx::lang::Object>& value) override;
    ::jxx::Ptr<::jxx::lang::Object> getAttribute(const ::jxx::Ptr<::jxx::lang::String>& name) const override;
    void setFeature(const ::jxx::Ptr<::jxx::lang::String>& name,::jxx::lang::jbool value) override;
    ::jxx::lang::jbool getFeature(const ::jxx::Ptr<::jxx::lang::String>& name) const override;
private:
    std::map<std::string,::jxx::Ptr<::jxx::lang::Object>> attributes_;
};
}
