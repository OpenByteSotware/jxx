#include <gtest/gtest.h>
#include "jxx.initialize.h"
#include "org/xml/sax/ext/jxx.org.xml.sax.ext.Attributes2.h"
#include "org/xml/sax/ext/jxx.org.xml.sax.ext.Attributes2Impl.h"
#include "org/xml/sax/ext/jxx.org.xml.sax.ext.ClassInitialization.h"
#include "org/xml/sax/ext/jxx.org.xml.sax.ext.DeclHandler.h"
#include "org/xml/sax/ext/jxx.org.xml.sax.ext.DefaultHandler2.h"
#include "org/xml/sax/ext/jxx.org.xml.sax.ext.EntityResolver2.h"
#include "org/xml/sax/ext/jxx.org.xml.sax.ext.LexicalHandler.h"
#include "org/xml/sax/ext/jxx.org.xml.sax.ext.Locator2.h"
#include "org/xml/sax/ext/jxx.org.xml.sax.ext.Locator2Impl.h"
TEST(JxxSaxExtClassInitialization, MetadataAndIdempotence) {
 namespace E=::jxx::org::xml::sax::ext;
 auto attributes2=::jxx::lang::InterfaceBase<E::Attributes2,::jxx::org::xml::sax::Attributes>::Class();
 ::jxx::initialize();
 EXPECT_NE(attributes2,nullptr);
 EXPECT_NE((::jxx::lang::ClassBase<E::Attributes2Impl,::jxx::org::xml::sax::helpers::AttributesImpl,E::Attributes2>::Class()),nullptr);
 EXPECT_NE((::jxx::lang::InterfaceBase<E::DeclHandler>::Class()),nullptr);
 EXPECT_NE((::jxx::lang::InterfaceBase<E::EntityResolver2,::jxx::org::xml::sax::EntityResolver>::Class()),nullptr);
 EXPECT_NE((::jxx::lang::InterfaceBase<E::LexicalHandler>::Class()),nullptr);
 EXPECT_NE((::jxx::lang::InterfaceBase<E::Locator2,::jxx::org::xml::sax::Locator>::Class()),nullptr);
 EXPECT_NE((::jxx::lang::ClassBase<E::DefaultHandler2,::jxx::org::xml::sax::helpers::DefaultHandler,E::DeclHandler,E::EntityResolver2,E::LexicalHandler>::Class()),nullptr);
 EXPECT_NE((::jxx::lang::ClassBase<E::Locator2Impl,::jxx::org::xml::sax::helpers::LocatorImpl,E::Locator2>::Class()),nullptr);
 EXPECT_NO_THROW(E::initializeClasses());
 EXPECT_NO_THROW(E::initializeClasses());
 EXPECT_EQ(attributes2,(::jxx::lang::InterfaceBase<E::Attributes2,::jxx::org::xml::sax::Attributes>::Class()));
}
