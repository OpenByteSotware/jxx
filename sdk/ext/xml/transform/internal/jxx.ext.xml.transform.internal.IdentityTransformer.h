#pragma once
#include <map>
#include <string>
#include "ext/xml/transform/jxx.ext.xml.transform.Transformer.h"
namespace jxx::ext::xml::transform::internal {
class IdentityTransformer final : public ::jxx::lang::ClassBase<IdentityTransformer,::jxx::ext::xml::transform::Transformer> {
public:
    void transform(const ::jxx::Ptr<::jxx::ext::xml::transform::Source>& source,const ::jxx::Ptr<::jxx::ext::xml::transform::Result>& result) override;
    void setOutputProperty(const ::jxx::Ptr<::jxx::lang::String>& name,const ::jxx::Ptr<::jxx::lang::String>& value) override;
    ::jxx::Ptr<::jxx::lang::String> getOutputProperty(const ::jxx::Ptr<::jxx::lang::String>& name) const override;
    void setOutputProperties(const ::jxx::Ptr<::jxx::util::Properties>& properties) override;
    ::jxx::Ptr<::jxx::util::Properties> getOutputProperties() const override;
    void setParameter(const ::jxx::Ptr<::jxx::lang::String>& name,const ::jxx::Ptr<::jxx::lang::Object>& value) override;
    ::jxx::Ptr<::jxx::lang::Object> getParameter(const ::jxx::Ptr<::jxx::lang::String>& name) const override;
    void clearParameters() override;
    void reset() override;
private:
    std::map<std::string,::jxx::Ptr<::jxx::lang::String>> outputProperties_;
    std::map<std::string,::jxx::Ptr<::jxx::lang::Object>> parameters_;
};
}
