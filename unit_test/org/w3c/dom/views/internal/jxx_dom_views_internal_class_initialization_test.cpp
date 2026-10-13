#include <gtest/gtest.h>
#include "jxx.initialize.h"
#include "org/w3c/dom/views/internal/jxx.org.w3c.dom.views.internal.ClassInitialization.h"
#include "org/w3c/dom/views/internal/jxx.org.w3c.dom.views.internal.AbstractViewImpl.h"
TEST(JxxDomViewsInternalClassInitialization, MetadataAndIdempotence) {
 using T=::jxx::org::w3c::dom::views::internal::AbstractViewImpl;
 auto before=::jxx::lang::ClassBase<T,::jxx::lang::Object,::jxx::org::w3c::dom::views::AbstractView>::Class();
 ::jxx::initialize(); EXPECT_NE(before,nullptr);
 EXPECT_NO_THROW(::jxx::org::w3c::dom::views::internal::initializeClasses());
 EXPECT_EQ(before,(::jxx::lang::ClassBase<T,::jxx::lang::Object,::jxx::org::w3c::dom::views::AbstractView>::Class()));
}
