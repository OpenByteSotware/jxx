#include <gtest/gtest.h>
#include "jxx.initialize.h"
#include "org/w3c/dom/bootstrap/jxx.org.w3c.dom.bootstrap.ClassInitialization.h"
#include "org/w3c/dom/bootstrap/jxx.org.w3c.dom.bootstrap.DOMImplementationRegistry.h"
namespace {
auto registryClass()
{
    return ::jxx::lang::ClassBase<
        ::jxx::org::w3c::dom::bootstrap::DOMImplementationRegistry,
        ::jxx::lang::Object>::Class();
}
}
TEST(JxxDomBootstrapClassInitialization, RegistryHasClassMetadata)
{
    ::jxx::initialize();
    EXPECT_NE(registryClass(), nullptr);
}
TEST(JxxDomBootstrapClassInitialization, RepeatedInitializationIsIdempotent)
{
    const auto before = registryClass();
    EXPECT_NO_THROW(::jxx::org::w3c::dom::bootstrap::initializeClasses());
    EXPECT_NO_THROW(::jxx::org::w3c::dom::bootstrap::initializeClasses());
    EXPECT_EQ(before, registryClass());
}
