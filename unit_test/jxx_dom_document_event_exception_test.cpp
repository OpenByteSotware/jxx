#include <gtest/gtest.h>
#include <type_traits>

#include "lang/jxx.lang.RuntimeException.h"
#include "lang/jxx.lang.String.h"
#include "org/w3c/dom/events/jxx.org.w3c.dom.events.DocumentEvent.h"
#include "org/w3c/dom/events/jxx.org.w3c.dom.events.EventException.h"

TEST(XmlDomDocumentEventExceptionStage, DocumentEventIsAbstractInterface) {
    EXPECT_TRUE((std::is_abstract_v<
        ::jxx::org::w3c::dom::events::DocumentEvent>));
    EXPECT_FALSE((std::is_base_of_v<
        ::jxx::lang::Object,
        ::jxx::org::w3c::dom::events::DocumentEvent>));
}

TEST(XmlDomDocumentEventExceptionStage, EventExceptionRetainsCodeAndMessage) {
    const ::jxx::org::w3c::dom::events::EventException exception(
        ::jxx::org::w3c::dom::events::EventException::UNSPECIFIED_EVENT_TYPE_ERR,
        ::jxx::NEW<::jxx::lang::String>("Event type is required"));
    EXPECT_EQ(exception.code, 0);
    EXPECT_STREQ(exception.what(), "Event type is required");
}

TEST(XmlDomDocumentEventExceptionStage, EventExceptionExtendsRuntimeException) {
    EXPECT_TRUE((std::is_base_of_v<
        ::jxx::lang::RuntimeException,
        ::jxx::org::w3c::dom::events::EventException>));
}
