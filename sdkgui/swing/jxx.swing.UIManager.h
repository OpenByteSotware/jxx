#pragma once

#include "lang/jxx.lang.Object.h"

namespace jxx::awt
{
    class Color;
    class Dimension;
    class Font;
    class Insets;
}

namespace jxx::swing
{
    class Icon;
    class UIDefaults;

    class UIManager final
    {
    public:
        static ::jxx::Ptr<UIDefaults> getDefaults();
        static ::jxx::Ptr<UIDefaults> getLookAndFeelDefaults();
        static ::jxx::Ptr<::jxx::lang::Object> get(
            const ::jxx::Ptr<::jxx::lang::Object>& key);
        static ::jxx::Ptr<::jxx::lang::Object> put(
            const ::jxx::Ptr<::jxx::lang::Object>& key,
            const ::jxx::Ptr<::jxx::lang::Object>& value);
        static ::jxx::Ptr<::jxx::lang::String> getString(
            const ::jxx::Ptr<::jxx::lang::Object>& key);
        static ::jxx::Ptr<::jxx::awt::Color> getColor(
            const ::jxx::Ptr<::jxx::lang::Object>& key);
        static ::jxx::Ptr<::jxx::awt::Font> getFont(
            const ::jxx::Ptr<::jxx::lang::Object>& key);
        static ::jxx::Ptr<Icon> getIcon(
            const ::jxx::Ptr<::jxx::lang::Object>& key);
        static ::jxx::Ptr<::jxx::awt::Insets> getInsets(
            const ::jxx::Ptr<::jxx::lang::Object>& key);
        static ::jxx::Ptr<::jxx::awt::Dimension> getDimension(
            const ::jxx::Ptr<::jxx::lang::Object>& key);
        static ::jxx::lang::jint getInt(
            const ::jxx::Ptr<::jxx::lang::Object>& key);
        static ::jxx::lang::jbool getBoolean(
            const ::jxx::Ptr<::jxx::lang::Object>& key);

    private:
        UIManager() = delete;
    };
}
