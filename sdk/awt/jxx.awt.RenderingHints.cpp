#include "awt/jxx.awt.RenderingHints.h"

#include <limits>

#include "lang/jxx.lang.IllegalArgumentException.h"
#include "lang/jxx.lang.Integer.h"
#include "lang/jxx.lang.NullPointerException.h"

namespace jxx::awt
{
    namespace
    {
        class HintValue final : public ::jxx::lang::ClassBase<HintValue, ::jxx::lang::Object>
        {
        public:
            using JxxSuper = ::jxx::lang::Object;
            explicit HintValue(const char* name) : name_(::jxx::NEW<::jxx::lang::String>(name)) {}
            ::jxx::Ptr<::jxx::lang::String> toString() const override { return name_; }
        private:
            ::jxx::Ptr<::jxx::lang::String> name_;
        };

        class StandardKey final : public RenderingHints::Key
        {
        public:
            StandardKey(::jxx::lang::jint id,
                const std::vector<::jxx::Ptr<::jxx::lang::Object>>& values,
                ::jxx::lang::jbool integerValue = false)
                : Key(id), values_(values), integerValue_(integerValue) {}
            ::jxx::lang::jbool isCompatibleValue(
                const ::jxx::Ptr<::jxx::lang::Object>& value) const override
            {
                if (integerValue_) return ::jxx::CAST<::jxx::lang::Integer>(value) != nullptr;
                for (const auto& allowed : values_) if (allowed == value) return true;
                return false;
            }
        private:
            std::vector<::jxx::Ptr<::jxx::lang::Object>> values_;
            ::jxx::lang::jbool integerValue_;
        };

        ::jxx::Ptr<::jxx::lang::Object> value(const char* name)
        {
            return ::jxx::CAST<::jxx::lang::Object>(::jxx::NEW<HintValue>(name));
        }
        ::jxx::Ptr<RenderingHints::Key> key(::jxx::lang::jint id,
            std::initializer_list<::jxx::Ptr<::jxx::lang::Object>> values)
        {
            return ::jxx::NEW<StandardKey>(id,
                std::vector<::jxx::Ptr<::jxx::lang::Object>>(values));
        }
    }

#define HINT_VALUE(NAME) ::jxx::Ptr<::jxx::lang::Object> RenderingHints::NAME = value(#NAME)
    HINT_VALUE(VALUE_ANTIALIAS_ON); HINT_VALUE(VALUE_ANTIALIAS_OFF); HINT_VALUE(VALUE_ANTIALIAS_DEFAULT);
    HINT_VALUE(VALUE_RENDER_SPEED); HINT_VALUE(VALUE_RENDER_QUALITY); HINT_VALUE(VALUE_RENDER_DEFAULT);
    HINT_VALUE(VALUE_DITHER_DISABLE); HINT_VALUE(VALUE_DITHER_ENABLE); HINT_VALUE(VALUE_DITHER_DEFAULT);
    HINT_VALUE(VALUE_TEXT_ANTIALIAS_ON); HINT_VALUE(VALUE_TEXT_ANTIALIAS_OFF); HINT_VALUE(VALUE_TEXT_ANTIALIAS_DEFAULT);
    HINT_VALUE(VALUE_TEXT_ANTIALIAS_GASP); HINT_VALUE(VALUE_TEXT_ANTIALIAS_LCD_HRGB); HINT_VALUE(VALUE_TEXT_ANTIALIAS_LCD_HBGR);
    HINT_VALUE(VALUE_TEXT_ANTIALIAS_LCD_VRGB); HINT_VALUE(VALUE_TEXT_ANTIALIAS_LCD_VBGR);
    HINT_VALUE(VALUE_FRACTIONALMETRICS_ON); HINT_VALUE(VALUE_FRACTIONALMETRICS_OFF); HINT_VALUE(VALUE_FRACTIONALMETRICS_DEFAULT);
    HINT_VALUE(VALUE_INTERPOLATION_NEAREST_NEIGHBOR); HINT_VALUE(VALUE_INTERPOLATION_BILINEAR); HINT_VALUE(VALUE_INTERPOLATION_BICUBIC);
    HINT_VALUE(VALUE_ALPHA_INTERPOLATION_SPEED); HINT_VALUE(VALUE_ALPHA_INTERPOLATION_QUALITY); HINT_VALUE(VALUE_ALPHA_INTERPOLATION_DEFAULT);
    HINT_VALUE(VALUE_COLOR_RENDER_SPEED); HINT_VALUE(VALUE_COLOR_RENDER_QUALITY); HINT_VALUE(VALUE_COLOR_RENDER_DEFAULT);
    HINT_VALUE(VALUE_STROKE_DEFAULT); HINT_VALUE(VALUE_STROKE_NORMALIZE); HINT_VALUE(VALUE_STROKE_PURE);
#undef HINT_VALUE

    ::jxx::Ptr<RenderingHints::Key> RenderingHints::KEY_ANTIALIASING = key(1, {VALUE_ANTIALIAS_ON, VALUE_ANTIALIAS_OFF, VALUE_ANTIALIAS_DEFAULT});
    ::jxx::Ptr<RenderingHints::Key> RenderingHints::KEY_RENDERING = key(2, {VALUE_RENDER_SPEED, VALUE_RENDER_QUALITY, VALUE_RENDER_DEFAULT});
    ::jxx::Ptr<RenderingHints::Key> RenderingHints::KEY_DITHERING = key(3, {VALUE_DITHER_DISABLE, VALUE_DITHER_ENABLE, VALUE_DITHER_DEFAULT});
    ::jxx::Ptr<RenderingHints::Key> RenderingHints::KEY_TEXT_ANTIALIASING = key(4, {VALUE_TEXT_ANTIALIAS_ON, VALUE_TEXT_ANTIALIAS_OFF, VALUE_TEXT_ANTIALIAS_DEFAULT, VALUE_TEXT_ANTIALIAS_GASP, VALUE_TEXT_ANTIALIAS_LCD_HRGB, VALUE_TEXT_ANTIALIAS_LCD_HBGR, VALUE_TEXT_ANTIALIAS_LCD_VRGB, VALUE_TEXT_ANTIALIAS_LCD_VBGR});
    ::jxx::Ptr<RenderingHints::Key> RenderingHints::KEY_TEXT_LCD_CONTRAST = ::jxx::NEW<StandardKey>(5, std::vector<::jxx::Ptr<::jxx::lang::Object>>{}, true);
    ::jxx::Ptr<RenderingHints::Key> RenderingHints::KEY_FRACTIONALMETRICS = key(6, {VALUE_FRACTIONALMETRICS_ON, VALUE_FRACTIONALMETRICS_OFF, VALUE_FRACTIONALMETRICS_DEFAULT});
    ::jxx::Ptr<RenderingHints::Key> RenderingHints::KEY_INTERPOLATION = key(7, {VALUE_INTERPOLATION_NEAREST_NEIGHBOR, VALUE_INTERPOLATION_BILINEAR, VALUE_INTERPOLATION_BICUBIC});
    ::jxx::Ptr<RenderingHints::Key> RenderingHints::KEY_ALPHA_INTERPOLATION = key(8, {VALUE_ALPHA_INTERPOLATION_SPEED, VALUE_ALPHA_INTERPOLATION_QUALITY, VALUE_ALPHA_INTERPOLATION_DEFAULT});
    ::jxx::Ptr<RenderingHints::Key> RenderingHints::KEY_COLOR_RENDERING = key(9, {VALUE_COLOR_RENDER_SPEED, VALUE_COLOR_RENDER_QUALITY, VALUE_COLOR_RENDER_DEFAULT});
    ::jxx::Ptr<RenderingHints::Key> RenderingHints::KEY_STROKE_CONTROL = key(10, {VALUE_STROKE_DEFAULT, VALUE_STROKE_NORMALIZE, VALUE_STROKE_PURE});

    RenderingHints::Key::Key(::jxx::lang::jint privateKey) : privateKey_(privateKey) {}
    ::jxx::lang::jint RenderingHints::Key::intKey() const { return privateKey_; }
    ::jxx::lang::jbool RenderingHints::Key::equals(const ::jxx::Ptr<::jxx::lang::Object>& object) const { return object.get() == this; }
    ::jxx::lang::jint RenderingHints::Key::hashCode() const { const auto value = reinterpret_cast<std::uintptr_t>(this); return static_cast<::jxx::lang::jint>(value ^ (value >> 32)); }
    RenderingHints::RenderingHints() = default;
    RenderingHints::RenderingHints(const ::jxx::Ptr<Key>& keyValue, const ::jxx::Ptr<::jxx::lang::Object>& hintValue) { put(keyValue, hintValue); }
    RenderingHints::RenderingHints(const RenderingHints& other) : values_(other.values_) {}
    std::size_t RenderingHints::KeyHash::operator()(const ::jxx::Ptr<Key>& keyValue) const noexcept { return std::hash<const void*>{}(keyValue.get()); }
    bool RenderingHints::KeyEqual::operator()(const ::jxx::Ptr<Key>& left, const ::jxx::Ptr<Key>& right) const noexcept { return left.get() == right.get(); }
    ::jxx::lang::jint RenderingHints::size() const { const auto maximum = static_cast<std::size_t>(std::numeric_limits<::jxx::lang::jint>::max()); return values_.size() > maximum ? std::numeric_limits<::jxx::lang::jint>::max() : static_cast<::jxx::lang::jint>(values_.size()); }
    ::jxx::lang::jbool RenderingHints::isEmpty() const { return values_.empty(); }
    ::jxx::lang::jbool RenderingHints::containsKey(const ::jxx::Ptr<Key>& keyValue) const { return keyValue != nullptr && values_.find(keyValue) != values_.end(); }
    ::jxx::Ptr<::jxx::lang::Object> RenderingHints::get(const ::jxx::Ptr<Key>& keyValue) const { const auto iterator = values_.find(keyValue); return iterator == values_.end() ? nullptr : iterator->second; }
    ::jxx::Ptr<::jxx::lang::Object> RenderingHints::put(const ::jxx::Ptr<Key>& keyValue, const ::jxx::Ptr<::jxx::lang::Object>& hintValue) { if (keyValue == nullptr) throw ::jxx::lang::NullPointerException("key"); if (!keyValue->isCompatibleValue(hintValue)) throw ::jxx::lang::IllegalArgumentException("value"); const auto previous = get(keyValue); values_[keyValue] = hintValue; return previous; }
    ::jxx::Ptr<::jxx::lang::Object> RenderingHints::remove(const ::jxx::Ptr<Key>& keyValue) { const auto previous = get(keyValue); if (keyValue != nullptr) values_.erase(keyValue); return previous; }
    void RenderingHints::clear() { values_.clear(); }
    void RenderingHints::add(const ::jxx::Ptr<RenderingHints>& hints) { if (hints == nullptr) return; for (const auto& entry : hints->values_) put(entry.first, entry.second); }
    ::jxx::Ptr<::jxx::lang::Object> RenderingHints::clone() const { return ::jxx::NEW<RenderingHints>(*this); }
    ::jxx::lang::jbool RenderingHints::equals(const ::jxx::Ptr<::jxx::lang::Object>& object) const { const auto other = ::jxx::CAST<RenderingHints>(object); return other != nullptr && values_ == other->values_; }
    ::jxx::lang::jint RenderingHints::hashCode() const { std::uint32_t hash = 0; for (const auto& entry : values_) hash += static_cast<std::uint32_t>(entry.first->hashCode()) ^ static_cast<std::uint32_t>(entry.second == nullptr ? 0 : entry.second->hashCode()); return static_cast<::jxx::lang::jint>(hash); }
}
