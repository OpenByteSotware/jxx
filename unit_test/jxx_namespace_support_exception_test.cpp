#include <gtest/gtest.h>

#include "lang/jxx.lang.EmptyStackException.h"
#include "lang/jxx.lang.IllegalStateException.h"
#include "org/xml/sax/helpers/jxx.org.xml.sax.helpers.NamespaceSupport.h"

TEST(XmlNamespaceSupportExceptions, PopRootThrowsEmptyStackException) {
    const auto support =
        ::jxx::NEW<
            ::jxx::org::xml::sax::helpers::NamespaceSupport>();

    EXPECT_THROW(
        support->popContext(),
        ::jxx::lang::EmptyStackException);
}

TEST(XmlNamespaceSupportExceptions, DeclarationModeRequiresRootContext) {
    const auto support =
        ::jxx::NEW<
            ::jxx::org::xml::sax::helpers::NamespaceSupport>();

    support->pushContext();

    EXPECT_THROW(
        support->setNamespaceDeclUris(true),
        ::jxx::lang::IllegalStateException);
}
