#pragma once
#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.String.h"
#include "lang/jxx_types.h"
namespace jxx::awt {
class Color : public ::jxx::lang::ClassBase<Color, ::jxx::lang::Object> {
public:
    using JxxSuper = ::jxx::lang::Object;
    using Super = ::jxx::lang::ClassBase<Color, JxxSuper>;
    static ::jxx::Ptr<Color> white; static ::jxx::Ptr<Color> WHITE;
    static ::jxx::Ptr<Color> lightGray; static ::jxx::Ptr<Color> LIGHT_GRAY;
    static ::jxx::Ptr<Color> gray; static ::jxx::Ptr<Color> GRAY;
    static ::jxx::Ptr<Color> darkGray; static ::jxx::Ptr<Color> DARK_GRAY;
    static ::jxx::Ptr<Color> black; static ::jxx::Ptr<Color> BLACK;
    static ::jxx::Ptr<Color> red; static ::jxx::Ptr<Color> RED;
    static ::jxx::Ptr<Color> pink; static ::jxx::Ptr<Color> PINK;
    static ::jxx::Ptr<Color> orange; static ::jxx::Ptr<Color> ORANGE;
    static ::jxx::Ptr<Color> yellow; static ::jxx::Ptr<Color> YELLOW;
    static ::jxx::Ptr<Color> green; static ::jxx::Ptr<Color> GREEN;
    static ::jxx::Ptr<Color> magenta; static ::jxx::Ptr<Color> MAGENTA;
    static ::jxx::Ptr<Color> cyan; static ::jxx::Ptr<Color> CYAN;
    static ::jxx::Ptr<Color> blue; static ::jxx::Ptr<Color> BLUE;
    explicit Color(::jxx::lang::jint rgb);
    Color(::jxx::lang::jint red, ::jxx::lang::jint green, ::jxx::lang::jint blue);
    Color(::jxx::lang::jint red, ::jxx::lang::jint green, ::jxx::lang::jint blue, ::jxx::lang::jint alpha);
    ::jxx::lang::jint getRed() const; ::jxx::lang::jint getGreen() const;
    ::jxx::lang::jint getBlue() const; ::jxx::lang::jint getAlpha() const;
    ::jxx::lang::jint getRGB() const;
    ::jxx::Ptr<Color> brighter() const; ::jxx::Ptr<Color> darker() const;
    ::jxx::lang::jbool equals(const ::jxx::Ptr<::jxx::lang::Object>& object) const override;
    ::jxx::lang::jint hashCode() const override;
    ::jxx::Ptr<::jxx::lang::String> toString() const override;
private:
    ::jxx::lang::jint value_;
};
}
