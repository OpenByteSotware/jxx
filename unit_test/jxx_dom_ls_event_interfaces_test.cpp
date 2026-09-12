#include <gtest/gtest.h>
#include <type_traits>

#include "org/w3c/dom/events/jxx.org.w3c.dom.events.Event.h"
#include "org/w3c/dom/ls/jxx.org.w3c.dom.ls.LSLoadEvent.h"
#include "org/w3c/dom/ls/jxx.org.w3c.dom.ls.LSProgressEvent.h"

TEST(XmlDomLsEventInterfacesStage, LoadAndProgressEventsExtendEvent) {
    EXPECT_TRUE((std::is_base_of_v<
        ::jxx::org::w3c::dom::events::Event,
        ::jxx::org::w3c::dom::ls::LSLoadEvent>));
    EXPECT_TRUE((std::is_base_of_v<
        ::jxx::org::w3c::dom::events::Event,
        ::jxx::org::w3c::dom::ls::LSProgressEvent>));
}

TEST(XmlDomLsEventInterfacesStage, EventPhaseConstantsMatchContract) {
    using Event = ::jxx::org::w3c::dom::events::Event;
    EXPECT_EQ(Event::CAPTURING_PHASE, 1);
    EXPECT_EQ(Event::AT_TARGET, 2);
    EXPECT_EQ(Event::BUBBLING_PHASE, 3);
}
