#pragma once

#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.String.h"

namespace jxx::swing
{
    template<typename M, typename I>
    class RowFilter : public ::jxx::lang::ClassBase<
        RowFilter<M, I>, ::jxx::lang::Object>
    {
    public:
        using JxxSuper = ::jxx::lang::Object;
        using Super = ::jxx::lang::ClassBase<RowFilter<M, I>, JxxSuper>;

        class Entry : public ::jxx::lang::ClassBase<
            Entry, ::jxx::lang::Object>
        {
        public:
            using JxxSuper = ::jxx::lang::Object;
            using Super = ::jxx::lang::ClassBase<Entry, JxxSuper>;

            Entry() : Super() {}
            ~Entry() override = default;

            virtual ::jxx::Ptr<M> getModel() const = 0;
            virtual ::jxx::lang::jint getValueCount() const = 0;
            virtual ::jxx::Ptr<::jxx::lang::Object> getValue(
                ::jxx::lang::jint index) const = 0;
            virtual ::jxx::Ptr<I> getIdentifier() const = 0;

            virtual ::jxx::Ptr<::jxx::lang::String> getStringValue(
                ::jxx::lang::jint index) const
            {
                const auto value = getValue(index);
                return value == nullptr
                    ? ::jxx::NEW<::jxx::lang::String>("")
                    : value->toString();
            }
        };

        RowFilter() : Super() {}
        ~RowFilter() override = default;

        virtual ::jxx::lang::jbool include(
            const ::jxx::Ptr<Entry>& entry) const = 0;
    };
}
