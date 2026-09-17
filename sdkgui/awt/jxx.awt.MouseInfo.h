#pragma once

#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx_types.h"

namespace jxx::awt
{
    class PointerInfo;

    class MouseInfo final :
        public ::jxx::lang::ClassBase<MouseInfo, ::jxx::lang::Object>
    {
    public:
        using JxxSuper = ::jxx::lang::Object;
        using Super = ::jxx::lang::ClassBase<MouseInfo, JxxSuper>;

        static ::jxx::Ptr<PointerInfo> getPointerInfo();
        static ::jxx::lang::jint getNumberOfButtons();

    private:
        MouseInfo() = delete;
    };
}
