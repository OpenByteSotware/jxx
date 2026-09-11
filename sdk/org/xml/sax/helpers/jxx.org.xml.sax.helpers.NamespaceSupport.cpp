#include "org/xml/sax/helpers/jxx.org.xml.sax.helpers.NamespaceSupport.h"

#include <cstddef>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#include "lang/jxx.lang.EmptyStackException.h"
#include "lang/jxx.lang.IllegalStateException.h"
#include "lang/jxx.lang.NullPointerException.h"
#include "lang/jxx.lang.String.h"
#include "util/jxx.util.NoSuchElementException.h"

namespace jxx::org::xml::sax::helpers {

namespace {

using String = ::jxx::lang::String;
using PrefixMap = std::unordered_map<std::string, std::string>;

class StringEnumeration final
    : public ::jxx::lang::ClassBase<
          StringEnumeration,
          ::jxx::lang::Object,
          ::jxx::util::Enumeration<String>> {
public:
    using JxxSuper = ::jxx::lang::Object;
    using Super = ::jxx::lang::ClassBase<
        StringEnumeration, JxxSuper, ::jxx::util::Enumeration<String>>;

    explicit StringEnumeration(std::vector<::jxx::Ptr<String>> values)
        : Super(), values_(std::move(values)) {
    }

    ::jxx::lang::jbool hasMoreElements() override {
        return index_ < values_.size();
    }

    ::jxx::Ptr<String> nextElement() override {
        if (!hasMoreElements()) {
            throw ::jxx::util::NoSuchElementException();
        }
        return values_[index_++];
    }

private:
    std::vector<::jxx::Ptr<String>> values_;
    std::size_t index_ = 0;
};

std::vector<::jxx::Ptr<String>> toStrings(
    const std::vector<std::string>& values) {
    std::vector<::jxx::Ptr<String>> result;
    result.reserve(values.size());
    for (const auto& value : values) {
        result.push_back(::jxx::NEW<String>(value));
    }
    return result;
}

} // namespace

struct NamespaceSupport::Impl {
    std::vector<PrefixMap> contexts;
    std::vector<std::vector<std::string>> declared;
    ::jxx::lang::jbool namespaceDeclUris = false;
};

::jxx::Ptr<::jxx::lang::String> NamespaceSupport::XMLNS =
    ::jxx::NEW<::jxx::lang::String>("http://www.w3.org/XML/1998/namespace");

::jxx::Ptr<::jxx::lang::String> NamespaceSupport::NSDECL =
    ::jxx::NEW<::jxx::lang::String>("http://www.w3.org/xmlns/2000/");

NamespaceSupport::NamespaceSupport()
    : Super(), impl_(std::make_shared<Impl>()) {
    reset();
}

NamespaceSupport::~NamespaceSupport() = default;

void NamespaceSupport::reset() {
    impl_->contexts.clear();
    impl_->declared.clear();
    PrefixMap root;
    root["xml"] = XMLNS->utf8();
    impl_->contexts.push_back(std::move(root));
    impl_->declared.emplace_back();
}

void NamespaceSupport::pushContext() {
    impl_->contexts.push_back(impl_->contexts.back());
    impl_->declared.emplace_back();
}

void NamespaceSupport::popContext() {
    if (impl_->contexts.size() <= 1) {
        throw ::jxx::lang::EmptyStackException();
    }
    impl_->contexts.pop_back();
    impl_->declared.pop_back();
}

::jxx::lang::jbool NamespaceSupport::declarePrefix(
    const ::jxx::Ptr<String>& prefix,
    const ::jxx::Ptr<String>& uri) {
    if (!prefix || !uri) {
        throw ::jxx::lang::NullPointerException();
    }
    const auto prefixValue = prefix->utf8();
    if (prefixValue == "xml" || prefixValue == "xmlns") {
        return false;
    }
    impl_->contexts.back()[prefixValue] = uri->utf8();
    auto& declared = impl_->declared.back();
    bool alreadyDeclared = false;
    for (const auto& value : declared) {
        if (value == prefixValue) {
            alreadyDeclared = true;
            break;
        }
    }
    if (!alreadyDeclared) {
        declared.push_back(prefixValue);
    }
    return true;
}

::jxx::Ptr<NamespaceSupport::StringArray> NamespaceSupport::processName(
    const ::jxx::Ptr<String>& qualifiedName,
    const ::jxx::Ptr<StringArray>& parts,
    ::jxx::lang::jbool isAttribute) {
    if (!qualifiedName || !parts) {
        throw ::jxx::lang::NullPointerException();
    }
    if (parts->length < 3) {
        return nullptr;
    }
    const std::string raw = qualifiedName->utf8();
    const auto separator = raw.find(':');
    std::string prefix;
    std::string local = raw;
    if (separator != std::string::npos) {
        if (separator == 0 || separator + 1 >= raw.size()) {
            return nullptr;
        }
        prefix = raw.substr(0, separator);
        local = raw.substr(separator + 1);
    }

    std::string uri;
    if (separator != std::string::npos) {
        const auto found = impl_->contexts.back().find(prefix);
        if (found == impl_->contexts.back().end()) {
            return nullptr;
        }
        uri = found->second;
    }
    else if (!isAttribute) {
        const auto found = impl_->contexts.back().find("");
        if (found != impl_->contexts.back().end()) {
            uri = found->second;
        }
    }
    else if (impl_->namespaceDeclUris && raw == "xmlns") {
        uri = NSDECL->utf8();
    }

    (*parts)[0] = ::jxx::NEW<String>(uri);
    (*parts)[1] = ::jxx::NEW<String>(local);
    (*parts)[2] = qualifiedName;
    return parts;
}

::jxx::Ptr<::jxx::lang::String> NamespaceSupport::getURI(
    const ::jxx::Ptr<String>& prefix) const {
    if (!prefix) {
        throw ::jxx::lang::NullPointerException();
    }
    const auto found = impl_->contexts.back().find(prefix->utf8());
    return found == impl_->contexts.back().end()
        ? nullptr
        : ::jxx::NEW<String>(found->second);
}

::jxx::Ptr<::jxx::lang::String> NamespaceSupport::getPrefix(
    const ::jxx::Ptr<String>& uri) const {
    if (!uri) {
        throw ::jxx::lang::NullPointerException();
    }
    const auto wanted = uri->utf8();
    for (const auto& entry : impl_->contexts.back()) {
        if (entry.second == wanted) {
            return ::jxx::NEW<String>(entry.first);
        }
    }
    return nullptr;
}

::jxx::Ptr<::jxx::util::Enumeration<::jxx::lang::String>>
NamespaceSupport::getPrefixes() const {
    std::vector<std::string> values;
    for (const auto& entry : impl_->contexts.back()) {
        values.push_back(entry.first);
    }
    return ::jxx::NEW<StringEnumeration>(toStrings(values));
}

::jxx::Ptr<::jxx::util::Enumeration<::jxx::lang::String>>
NamespaceSupport::getPrefixes(const ::jxx::Ptr<String>& uri) const {
    if (!uri) {
        throw ::jxx::lang::NullPointerException();
    }
    std::vector<std::string> values;
    const auto wanted = uri->utf8();
    for (const auto& entry : impl_->contexts.back()) {
        if (entry.second == wanted) {
            values.push_back(entry.first);
        }
    }
    return ::jxx::NEW<StringEnumeration>(toStrings(values));
}

::jxx::Ptr<::jxx::util::Enumeration<::jxx::lang::String>>
NamespaceSupport::getDeclaredPrefixes() const {
    return ::jxx::NEW<StringEnumeration>(
        toStrings(impl_->declared.back()));
}

void NamespaceSupport::setNamespaceDeclUris(::jxx::lang::jbool value) {
    if (impl_->contexts.size() != 1) {
        throw ::jxx::lang::IllegalStateException();
    }
    impl_->namespaceDeclUris = value;
    if (value) {
        impl_->contexts.front()["xmlns"] = NSDECL->utf8();
    }
    else {
        impl_->contexts.front().erase("xmlns");
    }
}

::jxx::lang::jbool NamespaceSupport::isNamespaceDeclUris() const {
    return impl_->namespaceDeclUris;
}

} // namespace jxx::org::xml::sax::helpers
