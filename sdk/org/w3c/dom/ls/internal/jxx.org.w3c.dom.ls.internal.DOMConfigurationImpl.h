#pragma once

#include <string>
#include <unordered_map>
#include <vector>

#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Object.h"
#include "org/w3c/dom/jxx.org.w3c.dom.DOMConfiguration.h"

namespace jxx::org::w3c::dom::ls::internal {

class DOMConfigurationImpl final
    : public ::jxx::lang::ClassBase<DOMConfigurationImpl, ::jxx::lang::Object,
          ::jxx::org::w3c::dom::DOMConfiguration> {
public:
    using JxxSuper = ::jxx::lang::Object;
    using Super = ::jxx::lang::ClassBase<DOMConfigurationImpl, JxxSuper,
        ::jxx::org::w3c::dom::DOMConfiguration>;

    explicit DOMConfigurationImpl(const std::vector<std::string>& parameterNames);
    ~DOMConfigurationImpl() override;

    void setParameter(const ::jxx::Ptr<::jxx::lang::String>& name,
        const ::jxx::Ptr<::jxx::lang::Object>& value) override;
    ::jxx::Ptr<::jxx::lang::Object> getParameter(
        const ::jxx::Ptr<::jxx::lang::String>& name) const override;
    ::jxx::lang::jbool canSetParameter(
        const ::jxx::Ptr<::jxx::lang::String>& name,
        const ::jxx::Ptr<::jxx::lang::Object>& value) const override;
    ::jxx::Ptr<::jxx::org::w3c::dom::DOMStringList> getParameterNames() const override;

private:
    std::vector<std::string> parameterNames_;
    std::unordered_map<std::string, ::jxx::Ptr<::jxx::lang::Object>> values_;
};

} // namespace jxx::org::w3c::dom::ls::internal
