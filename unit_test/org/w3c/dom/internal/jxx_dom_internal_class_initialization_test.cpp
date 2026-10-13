#include <gtest/gtest.h>
#include "jxx.initialize.h"
#include "org/w3c/dom/internal/jxx.org.w3c.dom.internal.ClassInitialization.h"
#include "org/w3c/dom/internal/jxx.org.w3c.dom.internal.DOMImplementationImpl.h"
#include "org/w3c/dom/internal/jxx.org.w3c.dom.internal.DOMImplementationSourceImpl.h"
TEST(JxxDomInternalClassInitialization, MetadataAndIdempotence) {
 using A=::jxx::org::w3c::dom::internal::DOMImplementationImpl;
 using B=::jxx::org::w3c::dom::internal::DOMImplementationSourceImpl;
 auto a=::jxx::lang::ClassBase<A,::jxx::lang::Object,::jxx::org::w3c::dom::DOMImplementation>::Class();
 auto b=::jxx::lang::ClassBase<B,::jxx::lang::Object,::jxx::org::w3c::dom::DOMImplementationSource>::Class();
 ::jxx::initialize(); EXPECT_NE(a,nullptr); EXPECT_NE(b,nullptr); EXPECT_NE(a,b);
 EXPECT_NO_THROW(::jxx::org::w3c::dom::internal::initializeClasses());
 EXPECT_EQ(a,(::jxx::lang::ClassBase<A,::jxx::lang::Object,::jxx::org::w3c::dom::DOMImplementation>::Class()));
}
