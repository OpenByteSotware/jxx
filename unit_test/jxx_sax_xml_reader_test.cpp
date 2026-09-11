#include <gtest/gtest.h>
#include <string>
#include <vector>
#include "io/jxx.io.StringReader.h"
#include "lang/jxx.lang.String.h"
#include "org/xml/sax/helpers/jxx.org.xml.sax.helpers.DefaultHandler.h"
#include "org/xml/sax/helpers/jxx.org.xml.sax.helpers.XMLReaderFactory.h"
#include "org/xml/sax/jxx.org.xml.sax.Attributes.h"
#include "org/xml/sax/jxx.org.xml.sax.InputSource.h"
#include "org/xml/sax/jxx.org.xml.sax.XMLReader.h"
namespace { class RecordingHandler final:public ::jxx::org::xml::sax::helpers::DefaultHandler {public:std::vector<std::string> events;void startDocument()override{events.push_back("start");}void startElement(const ::jxx::Ptr<::jxx::lang::String>& uri,const ::jxx::Ptr<::jxx::lang::String>& local,const ::jxx::Ptr<::jxx::lang::String>& q,const ::jxx::Ptr<::jxx::org::xml::sax::Attributes>& a)override{events.push_back("element:"+local->utf8()+":"+a->getValue(::jxx::NEW<::jxx::lang::String>("p:id"))->utf8());(void)uri;(void)q;}void endDocument()override{events.push_back("end");}}; }
TEST(XmlSaxReaderStage, EmitsNamespacedEvents){auto reader=::jxx::org::xml::sax::helpers::XMLReaderFactory::createXMLReader();auto handler=::jxx::NEW<RecordingHandler>();reader->setFeature(::jxx::NEW<::jxx::lang::String>("http://xml.org/sax/features/namespace-prefixes"),true);reader->setContentHandler(handler);auto input=::jxx::NEW<::jxx::org::xml::sax::InputSource>(::jxx::NEW<::jxx::io::StringReader>(::jxx::NEW<::jxx::lang::String>("<p:r xmlns:p='urn:t' p:id='7'>text</p:r>")));reader->parse(input);ASSERT_GE(handler->events.size(),3U);EXPECT_EQ(handler->events.front(),"start");EXPECT_EQ(handler->events[1],"element:r:7");EXPECT_EQ(handler->events.back(),"end");}
