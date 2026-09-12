#include <gtest/gtest.h>
#include <type_traits>

#include "org/w3c/dom/events/jxx.org.w3c.dom.events.EventListener.h"
#include "org/w3c/dom/events/jxx.org.w3c.dom.events.EventTarget.h"

TEST(XmlDomEventListenerTargetInterfacesStage, InterfacesRemainAbstract) {
    EXPECT_TRUE((std::is_abstract_v<
        ::jxx::org::w3c::dom::events::EventListener>));
    EXPECT_TRUE((std::is_abstract_v<
        ::jxx::org::w3c::dom::events::EventTarget>));
}

TEST(XmlDomEventListenerTargetInterfacesStage, InterfacesDoNotInheritObject) {
    EXPECT_FALSE((std::is_base_of_v<
        ::jxx::lang::Object,
        ::jxx::org::w3c::dom::events::EventListener>));
    EXPECT_FALSE((std::is_base_of_v<
        ::jxx::lang::Object,
        ::jxx::org::w3c::dom::events::EventTarget>));
}
