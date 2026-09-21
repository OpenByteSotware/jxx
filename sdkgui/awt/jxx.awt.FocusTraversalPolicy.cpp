#include "awt/jxx.awt.FocusTraversalPolicy.h"

#include "awt/jxx.awt.Window.h"

namespace jxx::awt
{
    FocusTraversalPolicy::FocusTraversalPolicy() : Super()
    {
    }

    ::jxx::Ptr<Component> FocusTraversalPolicy::getInitialComponent(
        const ::jxx::Ptr<Window>& window) const
    {
        return window == nullptr
            ? nullptr
            : getDefaultComponent(
                ::jxx::CAST<Container>(window));
    }
}
