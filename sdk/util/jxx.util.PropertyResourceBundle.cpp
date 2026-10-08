#include "util/jxx.util.PropertyResourceBundle.h"

#include "util/jxx.util.NoSuchElementException.h"
#include "lang/jxx.lang.NullPointerException.h"

namespace jxx::util {

PropertyResourceBundle::PropertyResourceBundle(
    const ::jxx::Ptr<::jxx::io::InputStream>& stream)
    : properties_(::jxx::NEW<Properties>()) {
    if (stream == nullptr) {
        throw ::jxx::lang::NullPointerException();
    }
    properties_->load(stream);
}

PropertyResourceBundle::PropertyResourceBundle(
    const ::jxx::Ptr<::jxx::io::Reader>& reader)
    : properties_(::jxx::NEW<Properties>()) {
    if (reader == nullptr) {
        throw ::jxx::lang::NullPointerException();
    }
    properties_->load(reader);
}

::jxx::Ptr<::jxx::lang::Object> PropertyResourceBundle::handleGetObject(
    const ::jxx::Ptr<::jxx::lang::String>& key) {
    if (key == nullptr) {
        throw ::jxx::lang::NullPointerException();
    }
    return ::jxx::CAST<::jxx::lang::Object>(properties_->getProperty(key));
}

::jxx::Ptr<Enumeration<::jxx::lang::String>>
PropertyResourceBundle::getLocalKeys() {
    std::vector<::jxx::Ptr<::jxx::lang::String>> keys;
    auto names = properties_->stringPropertyNames();
    if (names != nullptr) {
        auto iterator = names->iterator();
        while (iterator->hasNext()) {
            keys.push_back(iterator->next());
        }
    }
    return ::jxx::CAST<Enumeration<::jxx::lang::String>>(
        ::jxx::NEW<PropertyResourceBundleKeyEnumeration>(std::move(keys)));
}

PropertyResourceBundleKeyEnumeration::PropertyResourceBundleKeyEnumeration(
    std::vector<::jxx::Ptr<::jxx::lang::String>> keys)
    : keys_(std::move(keys)) {
}

::jxx::lang::jbool
PropertyResourceBundleKeyEnumeration::hasMoreElements() {
    return index_ < keys_.size();
}

::jxx::Ptr<::jxx::lang::String>
PropertyResourceBundleKeyEnumeration::nextElement() {
    if (!hasMoreElements()) {
        throw ::jxx::util::NoSuchElementException();
    }
    return keys_[index_++];
}

} // namespace jxx::util
