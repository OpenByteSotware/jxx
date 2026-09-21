#include "swing/jxx.swing.InputVerifier.h"

#include "swing/jxx.swing.JComponent.h"

namespace jxx::swing
{
    InputVerifier::InputVerifier() : Super()
    {
    }

    ::jxx::lang::jbool InputVerifier::shouldYieldFocus(
        const ::jxx::Ptr<JComponent>& input)
    {
        return verify(input);
    }
}
