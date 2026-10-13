#include <gtest/gtest.h>
#include "jxx.initialize.h"
#include "org/w3c/dom/events/internal/jxx.org.w3c.dom.events.internal.ClassInitialization.h"
#include "org/w3c/dom/events/internal/jxx.org.w3c.dom.events.internal.EventImpl.h"
TEST(JxxDomEventsInternalClassInitialization, MetadataAndIdempotence) {
 using T=::jxx::org::w3c::dom::events::internal::EventImpl;
 auto before=::jxx::lang::ClassBase<T,::jxx::lang::Object,::jxx::org::w3c::dom::events::Event>::Class();
 ::jxx::initialize(); EXPECT_NE(before,nullptr);
 EXPECT_NO_THROW(::jxx::org::w3c::dom::events::internal::initializeClasses());
 EXPECT_EQ(before,(::jxx::lang::ClassBase<T,::jxx::lang::Object,::jxx::org::w3c::dom::events::Event>::Class()));
}
