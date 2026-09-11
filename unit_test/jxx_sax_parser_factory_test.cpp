#include <gtest/gtest.h>
#include <string>
#include "ext/xml/parsers/jxx.ext.xml.parsers.SAXParser.h"
#include "ext/xml/parsers/jxx.ext.xml.parsers.SAXParserFactory.h"
#include "io/jxx.io.StringReader.h"
#include "lang/jxx.lang.String.h"
#include "org/xml/sax/helpers/jxx.org.xml.sax.helpers.DefaultHandler.h"
#include "org/xml/sax/jxx.org.xml.sax.InputSource.h"
namespace {class Handler final:public ::jxx::org::xml::sax::helpers::DefaultHandler{public:std::string name;void startElement(const ::jxx::Ptr<::jxx::lang::String>&,const ::jxx::Ptr<::jxx::lang::String>& l,const ::jxx::Ptr<::jxx::lang::String>&,const ::jxx::Ptr<::jxx::org::xml::sax::Attributes>&)override{name=l->utf8();}};}
TEST(XmlSaxParserFactoryStage, CreatesConfiguredParserAndParses){auto f=::jxx::ext::xml::parsers::SAXParserFactory::newInstance();f->setNamespaceAware(true);auto p=f->newSAXParser();ASSERT_NE(p,nullptr);EXPECT_TRUE(p->isNamespaceAware());auto h=::jxx::NEW<Handler>();auto source=::jxx::NEW<::jxx::org::xml::sax::InputSource>(::jxx::NEW<::jxx::io::StringReader>(::jxx::NEW<::jxx::lang::String>("<root/>")));p->parse(source,h);EXPECT_EQ(h->name,"root");}
