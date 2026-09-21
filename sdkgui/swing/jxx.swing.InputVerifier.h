#pragma once

#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Object.h"

namespace jxx::swing
{
    class JComponent;

    class InputVerifier : public ::jxx::lang::ClassBase<
        InputVerifier, ::jxx::lang::Object>
    {
    public:
        using JxxSuper = ::jxx::lang::Object;
        using Super = ::jxx::lang::ClassBase<InputVerifier, JxxSuper>;

        InputVerifier();
        ~InputVerifier() override = default;

        virtual ::jxx::lang::jbool verify(
            const ::jxx::Ptr<JComponent>& input) = 0;
        virtual ::jxx::lang::jbool shouldYieldFocus(
            const ::jxx::Ptr<JComponent>& input);
    };
}
