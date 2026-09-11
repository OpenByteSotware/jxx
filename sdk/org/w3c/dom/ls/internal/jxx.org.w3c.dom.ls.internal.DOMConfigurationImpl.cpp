#include "org/w3c/dom/ls/internal/jxx.org.w3c.dom.ls.internal.DOMConfigurationImpl.h"

#include <algorithm>
#include <cctype>
#include <utility>

#include "lang/jxx.lang.Boolean.h"
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
    ::jxx::lang::jbool contains(
        const ::jxx::Ptr<::jxx::lang::String>& value) const override {
        if (value == nullptr) {
            return false;
        }
        std::string candidate = value->utf8();
        std::transform(
            candidate.begin(),
            candidate.end(),
            candidate.begin(),
            [](unsigned char character) {
                return static_cast<char>(std::tolower(character));
            });
        return std::find(values_.begin(), values_.end(), candidate) != values_.end();
    }
private:
    std::vector<std::string> values_;
};

std::string keyOf(const ::jxx::Ptr<::jxx::lang::String>& name) {
    std::string key = name == nullptr ? std::string() : name->utf8();
    std::transform(key.begin(), key.end(), key.begin(),
        [](unsigned char value) {
            return static_cast<char>(std::tolower(value));
        });
    return key;
}

bool defaultBooleanValue(const std::string& name) {
    return name == "cdata-sections" ||
        name == "comments" ||
        name == "entities" ||
        name == "namespace-declarations" ||
        name == "namespaces" ||
        name == "well-formed" ||
        name == "xml-declaration";
}
} // namespace

DOMConfigurationImpl::DOMConfigurationImpl(const std::vector<std::string>& names)
    : parameterNames_(names) {
    for (const auto& name : parameterNames_) {
        values_[name] = ::jxx::CAST<::jxx::lang::Object>(
            ::jxx::lang::Boolean::valueOf(defaultBooleanValue(name)));
    }
}
DOMConfigurationImpl::~DOMConfigurationImpl() = default;

void DOMConfigurationImpl::setParameter(
    const ::jxx::Ptr<::jxx::lang::String>& name,
    const ::jxx::Ptr<::jxx::lang::Object>& value) {
    const auto key = keyOf(name);
    if (std::find(parameterNames_.begin(), parameterNames_.end(), key) == parameterNames_.end()) {
        throw ::jxx::org::w3c::dom::DOMException(
            ::jxx::org::w3c::dom::DOMException::NOT_FOUND_ERR,
            ::jxx::NEW<::jxx::lang::String>("Unsupported configuration parameter"));
    }
    if (::jxx::CAST<::jxx::lang::Boolean>(value) == nullptr) {
        throw ::jxx::org::w3c::dom::DOMException(
            ::jxx::org::w3c::dom::DOMException::TYPE_MISMATCH_ERR,
            ::jxx::NEW<::jxx::lang::String>("Configuration parameter requires Boolean"));
    }
    values_[key] = value;
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
    const ::jxx::Ptr<::jxx::lang::Object>& value) const {
    const auto key = keyOf(name);
    return std::find(parameterNames_.begin(), parameterNames_.end(), key) != parameterNames_.end() &&
        ::jxx::CAST<::jxx::lang::Boolean>(value) != nullptr;
}

::jxx::Ptr<::jxx::org::w3c::dom::DOMStringList>
DOMConfigurationImpl::getParameterNames() const {
    return ::jxx::CAST<::jxx::org::w3c::dom::DOMStringList>(
        ::jxx::NEW<DOMStringListImpl>(parameterNames_));
}
} // namespace jxx::org::w3c::dom::ls::internal
