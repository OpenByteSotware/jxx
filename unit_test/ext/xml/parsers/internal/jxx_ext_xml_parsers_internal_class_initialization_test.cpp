#include <gtest/gtest.h>

#include "ext/xml/parsers/internal/jxx.ext.xml.parsers.internal.ClassInitialization.h"
#include "ext/xml/parsers/internal/jxx.ext.xml.parsers.internal.PugiDocumentBuilder.h"
#include "ext/xml/parsers/internal/jxx.ext.xml.parsers.internal.PugiDocumentBuilderFactory.h"
#include "ext/xml/parsers/internal/jxx.ext.xml.parsers.internal.PugiSAXParser.h"
#include "jxx.initialize.h"

TEST(JxxExtXmlParsersInternalClassInitialization, MetadataTypesAreInitialized)
{
    ::jxx::initialize();

    namespace I = ::jxx::ext::xml::parsers::internal;
    namespace P = ::jxx::ext::xml::parsers;
    EXPECT_NE((::jxx::lang::ClassBase<I::PugiDocumentBuilder, P::DocumentBuilder>::Class()), nullptr);
    EXPECT_NE((::jxx::lang::ClassBase<I::PugiDocumentBuilderFactory, P::DocumentBuilderFactory>::Class()), nullptr);
    EXPECT_NE((::jxx::lang::ClassBase<I::PugiSAXParser, P::SAXParser>::Class()), nullptr);
}

TEST(JxxExtXmlParsersInternalClassInitialization, RepeatedInitializationIsIdempotent)
{
    namespace I = ::jxx::ext::xml::parsers::internal;
    namespace P = ::jxx::ext::xml::parsers;
    const auto before = ::jxx::lang::ClassBase<I::PugiDocumentBuilder, P::DocumentBuilder>::Class();

    EXPECT_NO_THROW(I::initializeClasses());
    EXPECT_NO_THROW(I::initializeClasses());

    EXPECT_EQ(before, (::jxx::lang::ClassBase<I::PugiDocumentBuilder, P::DocumentBuilder>::Class()));
}
