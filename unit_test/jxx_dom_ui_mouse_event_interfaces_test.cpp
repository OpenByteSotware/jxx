#include <gtest/gtest.h>
#include <type_traits>

#include "lang/jxx.lang.Object.h"
#include "org/w3c/dom/events/jxx.org.w3c.dom.events.Event.h"
#include "org/w3c/dom/events/jxx.org.w3c.dom.events.MouseEvent.h"
#include "org/w3c/dom/events/jxx.org.w3c.dom.events.UIEvent.h"
#include "org/w3c/dom/views/jxx.org.w3c.dom.views.AbstractView.h"
#include "org/w3c/dom/views/jxx.org.w3c.dom.views.DocumentView.h"

TEST(XmlDomUiMouseEventInterfacesStage, EventHierarchyMatchesContract) {
    EXPECT_TRUE((std::is_base_of_v<
        ::jxx::org::w3c::dom::events::Event,
        ::jxx::org::w3c::dom::events::UIEvent>));
    EXPECT_TRUE((std::is_base_of_v<
        ::jxx::org::w3c::dom::events::UIEvent,
        ::jxx::org::w3c::dom::events::MouseEvent>));
}

TEST(XmlDomUiMouseEventInterfacesStage, InterfacesRemainAbstract) {
    EXPECT_TRUE((std::is_abstract_v<
        ::jxx::org::w3c::dom::events::UIEvent>));
    EXPECT_TRUE((std::is_abstract_v<
        ::jxx::org::w3c::dom::events::MouseEvent>));
    EXPECT_TRUE((std::is_abstract_v<
        ::jxx::org::w3c::dom::views::AbstractView>));
    EXPECT_TRUE((std::is_abstract_v<
        ::jxx::org::w3c::dom::views::DocumentView>));
}

TEST(XmlDomUiMouseEventInterfacesStage, ViewInterfacesDoNotInheritObject) {
    EXPECT_FALSE((std::is_base_of_v<
        ::jxx::lang::Object,
        ::jxx::org::w3c::dom::views::AbstractView>));
    EXPECT_FALSE((std::is_base_of_v<
        ::jxx::lang::Object,
        ::jxx::org::w3c::dom::views::DocumentView>));
}
