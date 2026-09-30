#include <string>
#include "com/sun/net/httpserver/jxx.com.sun.net.httpserver.Headers.h"

#include "lang/jxx.lang.Exceptions.h"
#include "util/jxx.util.ArrayList.h"
#include "util/jxx.util.Iterator.h"
#include "util/jxx.util.MapEntry.h"
#include "com/sun/net/httpserver/internal/jxx.com.sun.net.httpserver.internal.ReadOnlyViews.h"
#include "com/sun/net/httpserver/internal/jxx.com.sun.net.httpserver.internal.ReadOnlyList.h"


namespace jxx::com::sun::net::httpserver {

Headers::Headers()
    : Super(), values_(::jxx::NEW<StorageType>())
{
}

void Headers::freezeInternal()
{
    frozen_ = true;
}

::jxx::lang::jbool Headers::isFrozenInternal() const noexcept
{
    return frozen_;
}

void Headers::ensureMutable_() const
{
    if (frozen_) {
        throw ::jxx::lang::UnsupportedOperationException();
    }
}

::jxx::lang::jint Headers::size()
{
    return values_->size();
}

::jxx::lang::jbool Headers::containsValue(
    const ::jxx::Ptr<::jxx::lang::Object>& value)
{
    return values_->containsValue(value);
}

::jxx::lang::jbool Headers::equals(
    const ::jxx::Ptr<::jxx::lang::Object>& other) const
{
    return values_->equals(other);
}

::jxx::lang::jint Headers::hashCode() const
{
    return values_->hashCode();
}

::jxx::Ptr<::jxx::lang::String> Headers::normalizeKey_(
    const ::jxx::Ptr<::jxx::lang::String>& key)
{
    if (key == nullptr) {
        throw ::jxx::lang::NullPointerException();
    }

    std::string text = key->utf8();
    if (text.empty()) {
        throw ::jxx::lang::IllegalArgumentException();
    }

    for (auto& character : text) {
        const unsigned char value = static_cast<unsigned char>(character);
        if (value <= 32U || value >= 127U || character == ':') {
            throw ::jxx::lang::IllegalArgumentException();
        }
        if (character >= 'A' && character <= 'Z') {
            character = static_cast<char>(character - 'A' + 'a');
        }
    }

    return ::jxx::NEW<::jxx::lang::String>(text.c_str());
}

::jxx::Ptr<::jxx::lang::String> Headers::normalizeObjectKey_(
    const ::jxx::Ptr<::jxx::lang::Object>& key)
{
    if (key == nullptr) {
        throw ::jxx::lang::NullPointerException();
    }

    auto stringKey = ::jxx::CAST<::jxx::lang::String>(key);
    if (stringKey == nullptr) {
        return nullptr;
    }

    return normalizeKey_(stringKey);
}

void Headers::validateValue_(
    const ::jxx::Ptr<::jxx::lang::String>& value)
{
    if (value == nullptr) {
        throw ::jxx::lang::NullPointerException();
    }

    const auto text = value->utf8();
    if (text.find('\r') != std::string::npos ||
        text.find('\n') != std::string::npos) {
        throw ::jxx::lang::IllegalArgumentException();
    }
}

void Headers::validateList_(
    const ::jxx::Ptr<ValueList>& values)
{
    if (values == nullptr) {
        throw ::jxx::lang::NullPointerException();
    }

    for (::jxx::lang::jint index = 0;
         index < values->size();
         ++index) {
        validateValue_(values->get(index));
    }
}

void Headers::add(
    const ::jxx::Ptr<::jxx::lang::String>& key,
    const ::jxx::Ptr<::jxx::lang::String>& value)
{
    ensureMutable_();
    validateValue_(value);
    auto normalized = normalizeKey_(key);
    auto values = values_->get(
        ::jxx::CAST<::jxx::lang::Object>(normalized));

    if (values == nullptr) {
        values = ::jxx::NEW<
            ::jxx::util::ArrayList<::jxx::lang::String>>();
        values_->put(normalized, values);
    }

    values->add(value);
}

void Headers::set(
    const ::jxx::Ptr<::jxx::lang::String>& key,
    const ::jxx::Ptr<::jxx::lang::String>& value)
{
    ensureMutable_();
    validateValue_(value);
    auto values = ::jxx::NEW<
        ::jxx::util::ArrayList<::jxx::lang::String>>();
    values->add(value);
    values_->put(normalizeKey_(key), values);
}

::jxx::Ptr<::jxx::lang::String> Headers::getFirst(
    const ::jxx::Ptr<::jxx::lang::String>& key)
{
    auto values = get(
        ::jxx::CAST<::jxx::lang::Object>(key));
    return values == nullptr || values->isEmpty()
        ? nullptr
        : values->get(0);
}

::jxx::lang::jbool Headers::containsKey(
    const ::jxx::Ptr<::jxx::lang::Object>& key)
{
    auto normalized = normalizeObjectKey_(key);
    return normalized != nullptr && values_->containsKey(
        ::jxx::CAST<::jxx::lang::Object>(normalized));
}

::jxx::Ptr<Headers::ValueList> Headers::get(
    const ::jxx::Ptr<::jxx::lang::Object>& key)
{
    auto normalized = normalizeObjectKey_(key);
    if (normalized == nullptr) {
        return nullptr;
    }
    auto value = values_->get(
        ::jxx::CAST<::jxx::lang::Object>(normalized));
    if (!frozen_ || value == nullptr) return value;
    return ::jxx::CAST<ValueList>(::jxx::NEW<::jxx::com::sun::net::httpserver::internal::ReadOnlyList<::jxx::lang::String>>(value));
}

::jxx::Ptr<Headers::ValueList> Headers::put(
    const ::jxx::Ptr<::jxx::lang::String>& key,
    const ::jxx::Ptr<ValueList>& value)
{
    ensureMutable_();
    validateList_(value);
    return values_->put(normalizeKey_(key), value);
}

::jxx::Ptr<Headers::ValueList> Headers::remove(
    const ::jxx::Ptr<::jxx::lang::Object>& key)
{
    ensureMutable_();
    auto normalized = normalizeObjectKey_(key);
    if (normalized == nullptr) {
        return nullptr;
    }
    return values_->remove(
        ::jxx::CAST<::jxx::lang::Object>(normalized));
}

::jxx::Ptr<::jxx::util::Set<::jxx::util::MapEntry<::jxx::lang::String, Headers::ValueList>>> Headers::entrySet()
{
    auto delegate = values_->entrySet();
    if (!frozen_) return delegate;
    return ::jxx::CAST<::jxx::util::Set<::jxx::util::MapEntry<::jxx::lang::String, ValueList>>>(
        ::jxx::NEW<::jxx::com::sun::net::httpserver::internal::ReadOnlyEntrySet<::jxx::lang::String, ValueList>>(delegate));
}

::jxx::Ptr<::jxx::util::Set<::jxx::lang::String>> Headers::keySet()
{
    auto delegate = values_->keySet();
    if (!frozen_) return delegate;
    return ::jxx::CAST<::jxx::util::Set<::jxx::lang::String>>(
        ::jxx::NEW<::jxx::com::sun::net::httpserver::internal::ReadOnlySet<::jxx::lang::String>>(delegate));
}

::jxx::Ptr<::jxx::util::Collection<Headers::ValueList>> Headers::values()
{
    auto delegate = values_->values();
    if (!frozen_) return delegate;
    return ::jxx::CAST<::jxx::util::Collection<ValueList>>(
        ::jxx::NEW<::jxx::com::sun::net::httpserver::internal::ReadOnlyCollection<ValueList>>(delegate));
}

void Headers::clear()
{
    ensureMutable_();
    values_->clear();
}

void Headers::putAll(
    const ::jxx::Ptr<
        ::jxx::util::Map<::jxx::lang::String, ValueList>>& source)
{
    ensureMutable_();
    if (source == nullptr) {
        throw ::jxx::lang::NullPointerException();
    }

    auto entries = source->entrySet()->iterator();
    while (entries->hasNext()) {
        auto entry = entries->next();
        put(entry->getKey(), entry->getValue());
    }
}

} // namespace jxx::com::sun::net::httpserver
