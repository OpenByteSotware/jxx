#include "org/w3c/dom/ls/internal/jxx.org.w3c.dom.ls.internal.DOMConfigurationImpl.h"

#include <algorithm>
#include <utility>

#include "lang/jxx.lang.String.h"
#include "org/w3c/dom/jxx.org.w3c.dom.DOMException.h"
#include "org/w3c/dom/jxx.org.w3c.dom.DOMStringList.h"

namespace jxx::org::w3c::dom::ls::internal {
namespace {
class DOMStringListImpl final
    : public ::jxx::lang::ClassBase<DOMStringListImpl, ::jxx::lang::Object,
          ::jxx::org::w3c::dom::DOMStringList> {
public:
    using JxxSuper = ::jxx::lang::Object;
    using Super = ::jxx::lang::ClassBase<DOMStringListImpl, JxxSuper,
        ::jxx::org::w3c::dom::DOMStringList>;
    explicit DOMStringListImpl(std::vector<std::string> values)
        : values_(std::move(values)) {}
    ::jxx::Ptr<::jxx::lang::String> item(::jxx::lang::jint index) const override {
        if (index < 0 || index >= static_cast<::jxx::lang::jint>(values_.size())) return nullptr;
        return ::jxx::NEW<::jxx::lang::String>(values_[static_cast<std::size_t>(index)].c_str());
    }
    ::jxx::lang::jint getLength() const override {
        return static_cast<::jxx::lang::jint>(values_.size());
    }
    ::jxx::lang::jbool contains(const ::jxx::Ptr<::jxx::lang::String>& value) const override {
        return value != nullptr &&
            std::find(values_.begin(), values_.end(), value->utf8()) != values_.end();
    }
private:
    std::vector<std::string> values_;
};

std::string keyOf(const ::jxx::Ptr<::jxx::lang::String>& name) {
    return name == nullptr ? std::string() : name->utf8();
}
} // namespace

DOMConfigurationImpl::DOMConfigurationImpl(const std::vector<std::string>& names)
    : parameterNames_(names) {}
DOMConfigurationImpl::~DOMConfigurationImpl() = default;

void DOMConfigurationImpl::setParameter(
    const ::jxx::Ptr<::jxx::lang::String>& name,
    const ::jxx::Ptr<::jxx::lang::Object>& value) {
    if (!canSetParameter(name, value)) {
        throw ::jxx::org::w3c::dom::DOMException(
            ::jxx::org::w3c::dom::DOMException::NOT_FOUND_ERR,
            ::jxx::NEW<::jxx::lang::String>("Unsupported configuration parameter"));
    }
    values_[keyOf(name)] = value;
}

::jxx::Ptr<::jxx::lang::Object> DOMConfigurationImpl::getParameter(
    const ::jxx::Ptr<::jxx::lang::String>& name) const {
    const auto key = keyOf(name);
    if (std::find(parameterNames_.begin(), parameterNames_.end(), key) == parameterNames_.end()) {
        throw ::jxx::org::w3c::dom::DOMException(
            ::jxx::org::w3c::dom::DOMException::NOT_FOUND_ERR,
            ::jxx::NEW<::jxx::lang::String>("Unsupported configuration parameter"));
    }
    const auto found = values_.find(key);
    return found == values_.end() ? nullptr : found->second;
}

::jxx::lang::jbool DOMConfigurationImpl::canSetParameter(
    const ::jxx::Ptr<::jxx::lang::String>& name,
    const ::jxx::Ptr<::jxx::lang::Object>&) const {
    const auto key = keyOf(name);
    return std::find(parameterNames_.begin(), parameterNames_.end(), key) != parameterNames_.end();
}

::jxx::Ptr<::jxx::org::w3c::dom::DOMStringList>
DOMConfigurationImpl::getParameterNames() const {
    return ::jxx::CAST<::jxx::org::w3c::dom::DOMStringList>(
        ::jxx::NEW<DOMStringListImpl>(parameterNames_));
}
} // namespace jxx::org::w3c::dom::ls::internal
