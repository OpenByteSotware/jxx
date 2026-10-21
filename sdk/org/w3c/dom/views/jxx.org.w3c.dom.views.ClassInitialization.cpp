#include "org/w3c/dom/views/jxx.org.w3c.dom.views.ClassInitialization.h"

#include <mutex>

#include "org/w3c/dom/views/jxx.org.w3c.dom.views.AbstractView.h"
#include "org/w3c/dom/views/jxx.org.w3c.dom.views.DocumentView.h"

#include "org/w3c/dom/views/internal/jxx.org.w3c.dom.views.internal.ClassInitialization.h"
namespace jxx::org::w3c::dom::views {

void initializeClasses()
{
    static std::once_flag flag;
    std::call_once(flag, [] {
        (void)AbstractView::JxxClassInfoMarker::Class();
        (void)DocumentView::JxxClassInfoMarker::Class();
    });
}


void initialize()
{
    initializeClasses();
    ::jxx::org::w3c::dom::views::internal::initialize();
}

} // namespace jxx::org::w3c::dom::views
