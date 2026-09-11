#include "org/w3c/dom/internal/jxx.org.w3c.dom.internal.DOMImplementationSourceImpl.h"

#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "lang/jxx.lang.String.h"
#include "org/w3c/dom/internal/jxx.org.w3c.dom.internal.DOMImplementationImpl.h"
#include "org/w3c/dom/jxx.org.w3c.dom.DOMImplementation.h"
#include "org/w3c/dom/jxx.org.w3c.dom.DOMImplementationList.h"

namespace jxx::org::w3c::dom::internal {

namespace {

class DOMImplementationListImpl final
    : public ::jxx::lang::ClassBase<
          DOMImplementationListImpl,
          ::jxx::lang::Object,
          ::jxx::org::w3c::dom::DOMImplementationList> {
public:
    using JxxSuper = ::jxx::lang::Object;
    using Super = ::jxx::lang::ClassBase<
        DOMImplementationListImpl,
        JxxSuper,
        ::jxx::org::w3c::dom::DOMImplementationList>;

    explicit DOMImplementationListImpl(
        std::vector<::jxx::Ptr<::jxx::org::w3c::dom::DOMImplementation>> values)
        : Super(), values_(std::move(values)) {
    }

    ::jxx::Ptr<::jxx::org::w3c::dom::DOMImplementation> item(
        ::jxx::lang::jint index) const override {
        if (index < 0 ||
            static_cast<std::size_t>(index) >= values_.size()) {
            return nullptr;
        }
        return values_[static_cast<std::size_t>(index)];
    }

    ::jxx::lang::jint getLength() const override {
        return static_cast<::jxx::lang::jint>(values_.size());
    }

private:
    std::vector<::jxx::Ptr<::jxx::org::w3c::dom::DOMImplementation>> values_;
};

bool isVersionToken(const std::string& value) {
    if (value.empty() || !std::isdigit(static_cast<unsigned char>(value[0]))) {
        return false;
    }
    for (const auto character : value) {
        if (!std::isdigit(static_cast<unsigned char>(character)) &&
            character != '.') {
            return false;
        }
    }
    return true;
}

bool supports(
    const ::jxx::Ptr<::jxx::org::w3c::dom::DOMImplementation>& value,
    const ::jxx::Ptr<::jxx::lang::String>& features) {
    if (features == nullptr || features->utf8().empty()) {
        return true;
    }

    std::istringstream stream(features->utf8());
    std::vector<std::string> tokens;
    std::string token;
    while (stream >> token) {
        tokens.push_back(token);
    }

    for (std::size_t index = 0; index < tokens.size(); ++index) {
        std::string feature = tokens[index];
        if (!feature.empty() && feature.front() == '+') {
            feature.erase(feature.begin());
        }

        std::string version;
        if (index + 1 < tokens.size() && isVersionToken(tokens[index + 1])) {
            version = tokens[++index];
        }

        if (!value->hasFeature(
                ::jxx::NEW<::jxx::lang::String>(feature),
                ::jxx::NEW<::jxx::lang::String>(version))) {
            return false;
        }
    }

    return true;
}

} // namespace

DOMImplementationSourceImpl::DOMImplementationSourceImpl()
    : Super() {
}

DOMImplementationSourceImpl::~DOMImplementationSourceImpl() = default;

::jxx::Ptr<::jxx::org::w3c::dom::DOMImplementation>
DOMImplementationSourceImpl::getDOMImplementation(
    const ::jxx::Ptr<::jxx::lang::String>& features) {
    const auto value = implementation();
    return supports(value, features) ? value : nullptr;
}

::jxx::Ptr<::jxx::org::w3c::dom::DOMImplementationList>
DOMImplementationSourceImpl::getDOMImplementationList(
    const ::jxx::Ptr<::jxx::lang::String>& features) {
    std::vector<::jxx::Ptr<::jxx::org::w3c::dom::DOMImplementation>> values;
    const auto value = implementation();
    if (supports(value, features)) {
        values.push_back(value);
    }
    return ::jxx::NEW<DOMImplementationListImpl>(std::move(values));
}

::jxx::Ptr<::jxx::org::w3c::dom::DOMImplementationSource>
implementationSource() {
    static const auto source =
        ::jxx::NEW<DOMImplementationSourceImpl>();
    return ::jxx::CAST<::jxx::org::w3c::dom::DOMImplementationSource>(source);
}

} // namespace jxx::org::w3c::dom::internal
