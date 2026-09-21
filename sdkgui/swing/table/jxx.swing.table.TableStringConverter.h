#pragma once

#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.String.h"

namespace jxx::swing::table
{
    class TableModel;

    class TableStringConverter : public ::jxx::lang::ClassBase<
        TableStringConverter, ::jxx::lang::Object>
    {
    public:
        using JxxSuper = ::jxx::lang::Object;
        using Super = ::jxx::lang::ClassBase<TableStringConverter, JxxSuper>;

        TableStringConverter();
        ~TableStringConverter() override = default;

        virtual ::jxx::Ptr<::jxx::lang::String> toString(
            const ::jxx::Ptr<TableModel>& model,
            ::jxx::lang::jint row,
            ::jxx::lang::jint column) const = 0;
    };
}
