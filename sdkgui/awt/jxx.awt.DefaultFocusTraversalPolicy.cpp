#include "awt/jxx.awt.DefaultFocusTraversalPolicy.h"

#include "awt/jxx.awt.Component.h"
#include "awt/jxx.awt.Container.h"
#include "awt/jxx.awt.Label.h"
#include "awt/jxx.awt.ScrollPane.h"
#include "awt/jxx.awt.Scrollbar.h"
#include "awt/jxx.awt.Window.h"

namespace jxx::awt
{
    DefaultFocusTraversalPolicy::DefaultFocusTraversalPolicy() : Super()
    {
    }

    ::jxx::lang::jbool DefaultFocusTraversalPolicy::accept(
        const ::jxx::Ptr<Component>& component) const
    {
        if (!JxxSuper::accept(component))
            return false;

        return ::jxx::CAST<Label>(component) == nullptr &&
            ::jxx::CAST<ScrollPane>(component) == nullptr &&
            ::jxx::CAST<Scrollbar>(component) == nullptr &&
            ::jxx::CAST<Window>(component) == nullptr;
    }
}
