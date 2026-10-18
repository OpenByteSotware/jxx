#pragma once

#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx_types.h"

namespace jxx::awt
{
    class ImageCapabilities :
        public ::jxx::lang::ClassBase<ImageCapabilities, ::jxx::lang::Object>
    {
    public:
        using JxxSuper = ::jxx::lang::Object;
        using Super = ::jxx::lang::ClassBase<ImageCapabilities, JxxSuper>;

        explicit ImageCapabilities(::jxx::lang::jbool accelerated);
        ~ImageCapabilities() override = default;

        virtual ::jxx::lang::jbool isAccelerated() const;
        virtual ::jxx::lang::jbool isTrueVolatile() const;
        ::jxx::Ptr<::jxx::lang::Object> clone() const override;

    private:
        ::jxx::lang::jbool accelerated_;
    };
}
