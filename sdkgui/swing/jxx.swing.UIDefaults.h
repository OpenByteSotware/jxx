#pragma once

#include <vector>

#include "io/jxx.io.SerializableI.h"
#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.String.h"

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

    class UIDefaults : public ::jxx::lang::ClassBase<
        UIDefaults, ::jxx::lang::Object, ::jxx::io::SerializableI>
    {
    public:
        using JxxSuper = ::jxx::lang::Object;
        using Super = ::jxx::lang::ClassBase<
            UIDefaults, JxxSuper, ::jxx::io::SerializableI>;

        UIDefaults();
        ~UIDefaults() override = default;

        ::jxx::Ptr<::jxx::lang::Object> get(
            const ::jxx::Ptr<::jxx::lang::Object>& key) const;
        ::jxx::Ptr<::jxx::lang::Object> put(
            const ::jxx::Ptr<::jxx::lang::Object>& key,
            const ::jxx::Ptr<::jxx::lang::Object>& value);
        ::jxx::Ptr<::jxx::lang::Object> remove(
            const ::jxx::Ptr<::jxx::lang::Object>& key);
        void clear();
        ::jxx::lang::jint size() const;
        ::jxx::lang::jbool containsKey(
            const ::jxx::Ptr<::jxx::lang::Object>& key) const;

        ::jxx::Ptr<::jxx::lang::String> getString(
            const ::jxx::Ptr<::jxx::lang::Object>& key) const;
        ::jxx::Ptr<::jxx::awt::Color> getColor(
            const ::jxx::Ptr<::jxx::lang::Object>& key) const;
        ::jxx::Ptr<::jxx::awt::Font> getFont(
            const ::jxx::Ptr<::jxx::lang::Object>& key) const;
        ::jxx::Ptr<Icon> getIcon(
            const ::jxx::Ptr<::jxx::lang::Object>& key) const;
        ::jxx::Ptr<::jxx::awt::Insets> getInsets(
            const ::jxx::Ptr<::jxx::lang::Object>& key) const;
        ::jxx::Ptr<::jxx::awt::Dimension> getDimension(
            const ::jxx::Ptr<::jxx::lang::Object>& key) const;
        ::jxx::lang::jint getInt(
            const ::jxx::Ptr<::jxx::lang::Object>& key) const;
        ::jxx::lang::jbool getBoolean(
            const ::jxx::Ptr<::jxx::lang::Object>& key) const;

        void writeObject(
            const ::jxx::Ptr<::jxx::io::ObjectOutputStream>& out) override;
        void readObject(
            const ::jxx::Ptr<::jxx::io::ObjectInputStream>& in) override;
        void readObjectNoData() override;

    private:
        struct Entry
        {
            ::jxx::Ptr<::jxx::lang::Object> key;
            ::jxx::Ptr<::jxx::lang::Object> value;
        };
        static ::jxx::lang::jbool sameKey(
            const ::jxx::Ptr<::jxx::lang::Object>& left,
            const ::jxx::Ptr<::jxx::lang::Object>& right);
        std::vector<Entry> entries_;
    };
}
