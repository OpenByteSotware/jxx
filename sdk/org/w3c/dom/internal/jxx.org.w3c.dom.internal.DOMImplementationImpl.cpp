#include "org/w3c/dom/internal/jxx.org.w3c.dom.internal.DOMImplementationImpl.h"

#include <algorithm>
#include <cctype>
#include <string>

#include "lang/jxx.lang.String.h"
#include "org/w3c/dom/jxx.org.w3c.dom.Document.h"
#include "org/w3c/dom/jxx.org.w3c.dom.DocumentType.h"

namespace jxx::org::w3c::dom::internal {

namespace {

std::string normalizedFeatureName(
    const ::jxx::Ptr<::jxx::lang::String>& feature) {

    if (feature == nullptr) {
        return {};
    }

    std::string result = feature->utf8();

    if (!result.empty() && result.front() == '+') {
        result.erase(result.begin());
    }

    std::transform(
        result.begin(),
        result.end(),
        result.begin(),
        [](unsigned char value) {
            return static_cast<char>(std::tolower(value));
        });

    return result;
}

bool isSupportedFeature(
    const std::string& feature) noexcept {

    return feature == "core" || feature == "xml";
}

bool isSupportedVersion(
    const ::jxx::Ptr<::jxx::lang::String>& version) {

    if (version == nullptr) {
        return true;
    }

    const std::string value = version->utf8();

    return value.empty() ||
        value == "1.0" ||
        value == "2.0" ||
        value == "3.0";
}

} // namespace

DOMImplementationImpl::DOMImplementationImpl()
    : Super() {
}

DOMImplementationImpl::~DOMImplementationImpl() = default;

::jxx::lang::jbool DOMImplementationImpl::hasFeature(
    const ::jxx::Ptr<::jxx::lang::String>& feature,
    const ::jxx::Ptr<::jxx::lang::String>& version) const {

    return static_cast<::jxx::lang::jbool>(
        isSupportedFeature(normalizedFeatureName(feature)) &&
        isSupportedVersion(version));
}

::jxx::Ptr<::jxx::org::w3c::dom::DocumentType>
DOMImplementationImpl::createDocumentType(
    const ::jxx::Ptr<::jxx::lang::String>& qualifiedName,
    const ::jxx::Ptr<::jxx::lang::String>& publicId,
    const ::jxx::Ptr<::jxx::lang::String>& systemId) {

    (void)qualifiedName;
    (void)publicId;
    (void)systemId;

    return nullptr;
}

::jxx::Ptr<::jxx::org::w3c::dom::Document>
DOMImplementationImpl::createDocument(
    const ::jxx::Ptr<::jxx::lang::String>& namespaceURI,
    const ::jxx::Ptr<::jxx::lang::String>& qualifiedName,
    const ::jxx::Ptr<
        ::jxx::org::w3c::dom::DocumentType>& documentType) {

    (void)namespaceURI;
    (void)qualifiedName;
    (void)documentType;

    return nullptr;
}

::jxx::Ptr<::jxx::lang::Object>
DOMImplementationImpl::getFeature(
    const ::jxx::Ptr<::jxx::lang::String>& feature,
    const ::jxx::Ptr<::jxx::lang::String>& version) {

    if (!hasFeature(feature, version)) {
        return nullptr;
    }

    return thisPtr();
}

::jxx::Ptr<::jxx::org::w3c::dom::DOMImplementation>
implementation() {

    static const auto instance =
        ::jxx::NEW<DOMImplementationImpl>();

    return ::jxx::CAST<
        ::jxx::org::w3c::dom::DOMImplementation>(instance);
}

} // namespace jxx::org::w3c::dom::internal
