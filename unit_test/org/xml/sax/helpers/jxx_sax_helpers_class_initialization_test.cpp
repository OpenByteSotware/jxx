#include <gtest/gtest.h>

#include "jxx.initialize.h"
#include "lang/jxx.lang.ClassInfo.h"
#include "org/xml/sax/helpers/jxx.org.xml.sax.helpers.AttributeListImpl.h"
#include "org/xml/sax/helpers/jxx.org.xml.sax.helpers.AttributesImpl.h"
#include "org/xml/sax/helpers/jxx.org.xml.sax.helpers.ClassInitialization.h"
#include "org/xml/sax/helpers/jxx.org.xml.sax.helpers.DefaultHandler.h"
#include "org/xml/sax/helpers/jxx.org.xml.sax.helpers.LocatorImpl.h"
#include "org/xml/sax/helpers/jxx.org.xml.sax.helpers.NamespaceSupport.h"
#include "org/xml/sax/helpers/jxx.org.xml.sax.helpers.ParserAdapter.h"
#include "org/xml/sax/helpers/jxx.org.xml.sax.helpers.ParserFactory.h"
#include "org/xml/sax/helpers/jxx.org.xml.sax.helpers.XMLFilterImpl.h"
#include "org/xml/sax/helpers/jxx.org.xml.sax.helpers.XMLReaderAdapter.h"
#include "org/xml/sax/helpers/jxx.org.xml.sax.helpers.XMLReaderFactory.h"

TEST(JxxSaxHelpersClassInitialization, MetadataTypesAreInitialized)
{
    ::jxx::initialize();

    using namespace ::jxx::org::xml::sax::helpers;
    EXPECT_NE(AttributeListImpl::JxxClassInfoMarker::Class(), nullptr);
    EXPECT_NE(AttributesImpl::JxxClassInfoMarker::Class(), nullptr);
    EXPECT_NE(DefaultHandler::JxxClassInfoMarker::Class(), nullptr);
    EXPECT_NE(LocatorImpl::JxxClassInfoMarker::Class(), nullptr);
    EXPECT_NE(NamespaceSupport::JxxClassInfoMarker::Class(), nullptr);
    EXPECT_NE(ParserAdapter::JxxClassInfoMarker::Class(), nullptr);
    EXPECT_NE(ParserFactory::JxxClassInfoMarker::Class(), nullptr);
    EXPECT_NE(XMLFilterImpl::JxxClassInfoMarker::Class(), nullptr);
    EXPECT_NE(XMLReaderAdapter::JxxClassInfoMarker::Class(), nullptr);
    EXPECT_NE(XMLReaderFactory::JxxClassInfoMarker::Class(), nullptr);
}

TEST(JxxSaxHelpersClassInitialization, RepeatedInitializationIsIdempotent)
{
    using namespace ::jxx::org::xml::sax::helpers;
    const auto before = DefaultHandler::JxxClassInfoMarker::Class();

    EXPECT_NO_THROW(initializeClasses());
    EXPECT_NO_THROW(initializeClasses());

    EXPECT_EQ(before, DefaultHandler::JxxClassInfoMarker::Class());
}
