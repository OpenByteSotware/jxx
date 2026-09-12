#include <gtest/gtest.h>

#include "lang/jxx.lang.String.h"
#include "org/w3c/dom/events/internal/jxx.org.w3c.dom.events.internal.EventImpl.h"

TEST(XmlDomBaseEventImplementationStage, InitializesBaseProperties) {
    const auto event = ::jxx::NEW<
        ::jxx::org::w3c::dom::events::internal::EventImpl>();
    event->initEvent(
        ::jxx::NEW<::jxx::lang::String>("sample"), true, true);
    ASSERT_NE(event->getType(), nullptr);
    EXPECT_EQ(event->getType()->utf8(), "sample");
    EXPECT_TRUE(event->getBubbles());
    EXPECT_TRUE(event->getCancelable());
    EXPECT_GT(event->getTimeStamp(), 0);
}

TEST(XmlDomBaseEventImplementationStage, PreventDefaultRequiresCancelableEvent) {
    const auto event = ::jxx::NEW<
        ::jxx::org::w3c::dom::events::internal::EventImpl>();
    event->initEvent(
        ::jxx::NEW<::jxx::lang::String>("first"), false, false);
    event->preventDefault();
    EXPECT_FALSE(event->isDefaultPrevented());
    event->initEvent(
        ::jxx::NEW<::jxx::lang::String>("second"), false, true);
    event->preventDefault();
    EXPECT_TRUE(event->isDefaultPrevented());
}

TEST(XmlDomBaseEventImplementationStage, StopPropagationIsTracked) {
    const auto event = ::jxx::NEW<
        ::jxx::org::w3c::dom::events::internal::EventImpl>();
    EXPECT_FALSE(event->isPropagationStopped());
    event->stopPropagation();
    EXPECT_TRUE(event->isPropagationStopped());
}
