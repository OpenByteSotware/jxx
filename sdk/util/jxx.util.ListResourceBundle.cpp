#include "util/jxx.util.ListResourceBundle.h"

#include "lang/jxx.lang.NullPointerException.h"
#include "util/jxx.util.NoSuchElementException.h"

namespace jxx::util {

void ListResourceBundle::loadLookup_() {
    std::call_once(loadFlag_, [this]() {
        auto contents = getContents();
        if (contents == nullptr) {
            throw ::jxx::lang::NullPointerException();
        }
        for (::jxx::lang::jint index = 0; index < contents->length; ++index) {
            auto keyObject = (*contents)[index][0];
            auto value = (*contents)[index][1];
            auto key = ::jxx::CAST<::jxx::lang::String>(keyObject);
            if (key == nullptr || value == nullptr) {
                throw ::jxx::lang::NullPointerException();
            }
            const auto text = key->utf8();
            if (lookup_.find(text) == lookup_.end()) {
                keys_.push_back(key);
            }
            lookup_[text] = value;
        }
    });
}

::jxx::Ptr<::jxx::lang::Object> ListResourceBundle::handleGetObject(
    const ::jxx::Ptr<::jxx::lang::String>& key) {
    if (key == nullptr) {
        throw ::jxx::lang::NullPointerException();
    }
    loadLookup_();
    const auto iterator = lookup_.find(key->utf8());
    return iterator == lookup_.end() ? nullptr : iterator->second;
}

::jxx::Ptr<Enumeration<::jxx::lang::String>>
ListResourceBundle::getLocalKeys() {
    loadLookup_();
    return ::jxx::CAST<Enumeration<::jxx::lang::String>>(
        ::jxx::NEW<ListResourceBundleKeyEnumeration>(keys_));
}

ListResourceBundleKeyEnumeration::ListResourceBundleKeyEnumeration(
    std::vector<::jxx::Ptr<::jxx::lang::String>> keys)
    : keys_(std::move(keys)) {
}

::jxx::lang::jbool ListResourceBundleKeyEnumeration::hasMoreElements() {
    return index_ < keys_.size();
}

::jxx::Ptr<::jxx::lang::String>
ListResourceBundleKeyEnumeration::nextElement() {
    if (!hasMoreElements()) {
        throw ::jxx::util::NoSuchElementException();
    }
    return keys_[index_++];
}

} // namespace jxx::util
