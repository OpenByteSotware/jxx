#include "org/w3c/dom/views/jxx.org.w3c.dom.views.ClassInitialization.h"

#include <mutex>

#include "org/w3c/dom/views/jxx.org.w3c.dom.views.AbstractView.h"
#include "org/w3c/dom/views/jxx.org.w3c.dom.views.DocumentView.h"

namespace jxx::org::w3c::dom::views {

void initializeClasses()
{
    static std::once_flag flag;
    std::call_once(flag, [] {
        (void)AbstractView::JxxClassInfoMarker::Class();
        (void)DocumentView::JxxClassInfoMarker::Class();
    });
}

} // namespace jxx::org::w3c::dom::views
