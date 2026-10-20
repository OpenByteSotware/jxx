#pragma once

#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.Runnable.h"
#include "lang/jxx_types.h"

namespace jxx::ext::swing {

class SwingUtilities final
    : public ::jxx::lang::ClassBase<SwingUtilities, ::jxx::lang::Object> {
public:
    using JxxSuper = ::jxx::lang::Object;
    using Super = ::jxx::lang::ClassBase<SwingUtilities, JxxSuper>;

    static void invokeLater(
        const ::jxx::Ptr<::jxx::lang::Runnable>& runnable);
    static void invokeAndWait(
        const ::jxx::Ptr<::jxx::lang::Runnable>& runnable);
    static ::jxx::lang::jbool isEventDispatchThread();

private:
    SwingUtilities() = delete;
};

} // namespace jxx::ext::swing
