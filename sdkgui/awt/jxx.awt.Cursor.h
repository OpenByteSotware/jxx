#pragma once

#include <array>

#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.String.h"
#include "lang/jxx_types.h"

namespace jxx::awt
{
    class Cursor : public ::jxx::lang::ClassBase<Cursor, ::jxx::lang::Object>
    {
    public:
        using JxxSuper = ::jxx::lang::Object;
        using Super = ::jxx::lang::ClassBase<Cursor, JxxSuper>;

        static constexpr ::jxx::lang::jint DEFAULT_CURSOR = 0;
        static constexpr ::jxx::lang::jint CROSSHAIR_CURSOR = 1;
        static constexpr ::jxx::lang::jint TEXT_CURSOR = 2;
        static constexpr ::jxx::lang::jint WAIT_CURSOR = 3;
        static constexpr ::jxx::lang::jint SW_RESIZE_CURSOR = 4;
        static constexpr ::jxx::lang::jint SE_RESIZE_CURSOR = 5;
        static constexpr ::jxx::lang::jint NW_RESIZE_CURSOR = 6;
        static constexpr ::jxx::lang::jint NE_RESIZE_CURSOR = 7;
        static constexpr ::jxx::lang::jint N_RESIZE_CURSOR = 8;
        static constexpr ::jxx::lang::jint S_RESIZE_CURSOR = 9;
        static constexpr ::jxx::lang::jint W_RESIZE_CURSOR = 10;
        static constexpr ::jxx::lang::jint E_RESIZE_CURSOR = 11;
        static constexpr ::jxx::lang::jint HAND_CURSOR = 12;
        static constexpr ::jxx::lang::jint MOVE_CURSOR = 13;
        static constexpr ::jxx::lang::jint CUSTOM_CURSOR = -1;

        explicit Cursor(::jxx::lang::jint type);
        ~Cursor() override = default;

        static ::jxx::Ptr<Cursor> getDefaultCursor();
        static ::jxx::Ptr<Cursor> getPredefinedCursor(::jxx::lang::jint type);
        static ::jxx::Ptr<Cursor> getSystemCustomCursor(
            const ::jxx::Ptr<::jxx::lang::String>& name);

        ::jxx::lang::jint getType() const;
        ::jxx::Ptr<::jxx::lang::String> getName() const;
        ::jxx::Ptr<::jxx::lang::String> toString() const override;

    protected:
        explicit Cursor(const ::jxx::Ptr<::jxx::lang::String>& name);
        ::jxx::Ptr<::jxx::lang::String> name;

    private:
        static ::jxx::Ptr<Cursor> createCustom(
            const ::jxx::Ptr<::jxx::lang::String>& name);
        static const char* predefinedName(::jxx::lang::jint type);
        static std::array<::jxx::Ptr<Cursor>, 14> predefined_;
        ::jxx::lang::jint type_;
    };
}
