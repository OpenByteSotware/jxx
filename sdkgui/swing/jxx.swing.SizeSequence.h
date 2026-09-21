#pragma once

#include <vector>

#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.buildin_array.h"

namespace jxx::swing
{
    class SizeSequence : public ::jxx::lang::ClassBase<
        SizeSequence, ::jxx::lang::Object>
    {
    public:
        using JxxSuper = ::jxx::lang::Object;
        using Super = ::jxx::lang::ClassBase<SizeSequence, JxxSuper>;
        using IntArray = ::jxx::Ptr<::jxx::lang::JxxArray<
            ::jxx::lang::jint, 1>>;

        SizeSequence();
        explicit SizeSequence(::jxx::lang::jint entries);
        SizeSequence(
            ::jxx::lang::jint entries,
            ::jxx::lang::jint value);
        explicit SizeSequence(const IntArray& sizes);

        void setSizes(::jxx::lang::jint length);
        void setSizes(const IntArray& sizes);
        IntArray getSizes() const;
        ::jxx::lang::jint getPosition(::jxx::lang::jint index) const;
        ::jxx::lang::jint getIndex(::jxx::lang::jint position) const;
        ::jxx::lang::jint getSize(::jxx::lang::jint index) const;
        void setSize(
            ::jxx::lang::jint index,
            ::jxx::lang::jint size);
        void insertEntries(
            ::jxx::lang::jint start,
            ::jxx::lang::jint length,
            ::jxx::lang::jint value);
        void removeEntries(
            ::jxx::lang::jint start,
            ::jxx::lang::jint length);

    private:
        void checkIndex(::jxx::lang::jint index) const;
        static void checkLength(::jxx::lang::jint length);
        std::vector<::jxx::lang::jint> sizes_;
    };
}
