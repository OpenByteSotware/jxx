#include <gtest/gtest.h>
#include "ext/xml/bind/annotation/jxx.ext.xml.bind.annotation.XmlAccessType.h"
#include "ext/xml/bind/annotation/jxx.ext.xml.bind.annotation.XmlAccessorType.h"
#include "ext/xml/bind/annotation/jxx.ext.xml.bind.annotation.XmlElement.h"
#include "ext/xml/bind/annotation/jxx.ext.xml.bind.annotation.XmlRootElement.h"
#include "ext/xml/bind/annotation/jxx.ext.xml.bind.annotation.XmlType.h"
namespace {
TEST(JaxbAnnotationCompatibilityTest, StoresGeneratedSourceMetadata){
 using namespace ::jxx::ext::xml::bind::annotation;
 auto access=::jxx::NEW<XmlAccessorType>(XmlAccessType::FIELD);
 auto element=::jxx::NEW<XmlElement>(::jxx::NEW<::jxx::lang::String>("port"),true,false);
 auto root=::jxx::NEW<XmlRootElement>(::jxx::NEW<::jxx::lang::String>("device"));
 EXPECT_TRUE(access->value()==XmlAccessType::FIELD);
 EXPECT_TRUE(element->name()->utf8()=="port"); EXPECT_TRUE(element->required());
 EXPECT_TRUE(root->name()->utf8()=="device");
 EXPECT_TRUE(XmlType::DEFAULT()->utf8()=="##default");
}
}
