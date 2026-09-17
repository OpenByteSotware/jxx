#include "awt/jxx.awt.RenderingHints.h"

#include <limits>

#include "lang/jxx.lang.IllegalArgumentException.h"
#include "lang/jxx.lang.NullPointerException.h"

namespace jxx::awt
{
    RenderingHints::Key::Key(::jxx::lang::jint privateKey)
        : privateKey_(privateKey)
    {
    }

    ::jxx::lang::jint RenderingHints::Key::intKey() const
    {
        return privateKey_;
    }

    ::jxx::lang::jbool RenderingHints::Key::equals(
        const ::jxx::Ptr<::jxx::lang::Object>& object) const
    {
        return object.get() == this;
    }

    ::jxx::lang::jint RenderingHints::Key::hashCode() const
    {
        const auto value = reinterpret_cast<std::uintptr_t>(this);
        return static_cast<::jxx::lang::jint>(value ^ (value >> 32));
    }

    RenderingHints::RenderingHints() = default;

    RenderingHints::RenderingHints(const ::jxx::Ptr<Key>& key,
        const ::jxx::Ptr<::jxx::lang::Object>& value)
    {
        put(key, value);
    }

    RenderingHints::RenderingHints(const RenderingHints& other)
        : values_(other.values_)
    {
    }

    std::size_t RenderingHints::KeyHash::operator()(
        const ::jxx::Ptr<Key>& key) const noexcept
    {
        return std::hash<const void*>{}(key.get());
    }

    bool RenderingHints::KeyEqual::operator()(
        const ::jxx::Ptr<Key>& left,
        const ::jxx::Ptr<Key>& right) const noexcept
    {
        return left.get() == right.get();
    }

    ::jxx::lang::jint RenderingHints::size() const
    {
        const auto maximum = static_cast<std::size_t>(
            std::numeric_limits<::jxx::lang::jint>::max());
        return values_.size() > maximum
            ? std::numeric_limits<::jxx::lang::jint>::max()
            : static_cast<::jxx::lang::jint>(values_.size());
    }

    ::jxx::lang::jbool RenderingHints::isEmpty() const
    {
        return values_.empty();
    }

    ::jxx::lang::jbool RenderingHints::containsKey(
        const ::jxx::Ptr<Key>& key) const
    {
        return key != nullptr && values_.find(key) != values_.end();
    }

    ::jxx::Ptr<::jxx::lang::Object> RenderingHints::get(
        const ::jxx::Ptr<Key>& key) const
    {
        const auto iterator = values_.find(key);
        return iterator == values_.end() ? nullptr : iterator->second;
    }

    ::jxx::Ptr<::jxx::lang::Object> RenderingHints::put(
        const ::jxx::Ptr<Key>& key,
        const ::jxx::Ptr<::jxx::lang::Object>& value)
    {
        if (key == nullptr) throw ::jxx::lang::NullPointerException("key");
        if (!key->isCompatibleValue(value))
            throw ::jxx::lang::IllegalArgumentException("value");
        const auto previous = get(key);
        values_[key] = value;
        return previous;
    }

    ::jxx::Ptr<::jxx::lang::Object> RenderingHints::remove(
        const ::jxx::Ptr<Key>& key)
    {
        const auto previous = get(key);
        if (key != nullptr) values_.erase(key);
        return previous;
    }

    void RenderingHints::clear() { values_.clear(); }

    void RenderingHints::add(const ::jxx::Ptr<RenderingHints>& hints)
    {
        if (hints == nullptr) return;
        for (const auto& entry : hints->values_)
            put(entry.first, entry.second);
    }

    ::jxx::Ptr<::jxx::lang::Object> RenderingHints::clone() const
    {
        return ::jxx::NEW<RenderingHints>(*this);
    }

    ::jxx::lang::jbool RenderingHints::equals(
        const ::jxx::Ptr<::jxx::lang::Object>& object) const
    {
        const auto other = ::jxx::CAST<RenderingHints>(object);
        return other != nullptr && values_ == other->values_;
    }

    ::jxx::lang::jint RenderingHints::hashCode() const
    {
        std::uint32_t hash = 0;
        for (const auto& entry : values_)
            hash += static_cast<std::uint32_t>(entry.first->hashCode()) ^
                static_cast<std::uint32_t>(
                    entry.second == nullptr ? 0 : entry.second->hashCode());
        return static_cast<::jxx::lang::jint>(hash);
    }
}
