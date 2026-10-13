#include <gtest/gtest.h>
#include "jxx.initialize.h"
#include "org/w3c/dom/ls/internal/jxx.org.w3c.dom.ls.internal.ClassInitialization.h"
#include "org/w3c/dom/ls/internal/jxx.org.w3c.dom.ls.internal.DOMConfigurationImpl.h"
#include "org/w3c/dom/ls/internal/jxx.org.w3c.dom.ls.internal.DOMImplementationLSImpl.h"
#include "org/w3c/dom/ls/internal/jxx.org.w3c.dom.ls.internal.LSInputImpl.h"
#include "org/w3c/dom/ls/internal/jxx.org.w3c.dom.ls.internal.LSOutputImpl.h"
#include "org/w3c/dom/ls/internal/jxx.org.w3c.dom.ls.internal.LSParserImpl.h"
#include "org/w3c/dom/ls/internal/jxx.org.w3c.dom.ls.internal.LSSerializerImpl.h"
TEST(JxxDomLsInternalClassInitialization, MetadataAndIdempotence) {
 namespace I=::jxx::org::w3c::dom::ls::internal;
 auto input=::jxx::lang::ClassBase<I::LSInputImpl,::jxx::lang::Object,::jxx::org::w3c::dom::ls::LSInput>::Class();
 ::jxx::initialize();
 EXPECT_NE(input,nullptr);
 EXPECT_NE((::jxx::lang::ClassBase<I::DOMConfigurationImpl,::jxx::lang::Object,::jxx::org::w3c::dom::DOMConfiguration>::Class()),nullptr);
 EXPECT_NE((::jxx::lang::ClassBase<I::DOMImplementationLSImpl,::jxx::lang::Object,::jxx::org::w3c::dom::ls::DOMImplementationLS>::Class()),nullptr);
 EXPECT_NE((::jxx::lang::ClassBase<I::LSOutputImpl,::jxx::lang::Object,::jxx::org::w3c::dom::ls::LSOutput>::Class()),nullptr);
 EXPECT_NE((::jxx::lang::ClassBase<I::LSParserImpl,::jxx::lang::Object,::jxx::org::w3c::dom::ls::LSParser>::Class()),nullptr);
 EXPECT_NE((::jxx::lang::ClassBase<I::LSSerializerImpl,::jxx::lang::Object,::jxx::org::w3c::dom::ls::LSSerializer>::Class()),nullptr);
 EXPECT_NO_THROW(I::initializeClasses());
 EXPECT_EQ(input,(::jxx::lang::ClassBase<I::LSInputImpl,::jxx::lang::Object,::jxx::org::w3c::dom::ls::LSInput>::Class()));
}
