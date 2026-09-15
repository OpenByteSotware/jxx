#pragma once
#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.String.h"
#include "lang/jxx_types.h"
namespace jxx::awt {
class Font : public ::jxx::lang::ClassBase<Font, ::jxx::lang::Object> {
public:
    using JxxSuper=::jxx::lang::Object; using Super=::jxx::lang::ClassBase<Font,JxxSuper>;
    static constexpr ::jxx::lang::jint PLAIN=0, BOLD=1, ITALIC=2;
    Font(const ::jxx::Ptr<::jxx::lang::String>& name, ::jxx::lang::jint style, ::jxx::lang::jint size);
    ::jxx::Ptr<::jxx::lang::String> getName() const; ::jxx::Ptr<::jxx::lang::String> getFamily() const;
    ::jxx::lang::jint getStyle() const; ::jxx::lang::jint getSize() const; ::jxx::lang::jfloat getSize2D() const;
    ::jxx::lang::jbool isPlain() const; ::jxx::lang::jbool isBold() const; ::jxx::lang::jbool isItalic() const;
    ::jxx::Ptr<Font> deriveFont(::jxx::lang::jint style) const; ::jxx::Ptr<Font> deriveFont(::jxx::lang::jfloat size) const;
    ::jxx::Ptr<Font> deriveFont(::jxx::lang::jint style, ::jxx::lang::jfloat size) const;
    ::jxx::lang::jbool equals(const ::jxx::Ptr<::jxx::lang::Object>& object) const override;
    ::jxx::lang::jint hashCode() const override; ::jxx::Ptr<::jxx::lang::String> toString() const override;
private:
    ::jxx::Ptr<::jxx::lang::String> name_; ::jxx::lang::jint style_; ::jxx::lang::jfloat size_;
}; }
