#include <gtest/gtest.h>
#include <string>
#include "ext/xml/parsers/jxx.ext.xml.parsers.SAXParserFactory.h"
#include "ext/xml/parsers/jxx.ext.xml.parsers.SAXParser.h"
#include "io/jxx.io.StringReader.h"
#include "org/xml/sax/jxx.org.xml.sax.DocumentHandler.h"
#include "org/xml/sax/jxx.org.xml.sax.InputSource.h"
#include "lang/jxx.lang.String.h"
#include "ext/xml/parsers/jxx.ext.xml.parsers.DocumentBuilderFactory.h"
#include "org/xml/sax/jxx.org.xml.sax.Parser.h"

namespace
{
	class LegacyHandler final :public ::jxx::lang::ClassBase<LegacyHandler, ::jxx::lang::Object, ::jxx::org::xml::sax::DocumentHandler>
	{
	public:std::string name; void setDocumentLocator(const ::jxx::Ptr<::jxx::org::xml::sax::Locator>&)override
	{
	}void startDocument()override
	{
	}
	
	void endDocument()override
	{
	}
	
	void startElement(const ::jxx::Ptr<::jxx::lang::String>& n, 
		const ::jxx::Ptr<::jxx::org::xml::sax::AttributeList>&)override
	{
		name = n->utf8();
	}void endElement(const ::jxx::Ptr<::jxx::lang::String>&)override
	{
	}void characters(const ::jxx::lang::CharArray&, ::jxx::lang::jint, ::jxx::lang::jint)override
	{
	}void ignorableWhitespace(const ::jxx::lang::CharArray&, ::jxx::lang::jint, ::jxx::lang::jint)override
	{
	}void processingInstruction(const ::jxx::Ptr<::jxx::lang::String>&, const ::jxx::Ptr<::jxx::lang::String>&)override
	{
	}
	};
}
TEST(XmlSax1Stage, ParserAdapterForwardsQualifiedNames)
{
	auto parser = 
		::jxx::ext::xml::parsers::SAXParserFactory::newInstance()->newSAXParser()->getParser();
	ASSERT_NE(parser, nullptr); auto handler = ::jxx::NEW<LegacyHandler>(); 
	parser->setDocumentHandler(handler); 
	auto input = 
		::jxx::NEW<::jxx::org::xml::sax::InputSource>(
			::jxx::NEW<::jxx::io::StringReader>(
				::jxx::NEW<::jxx::lang::String>("<p:root xmlns:p='urn:test'/>"))); 
	parser->parse(input); EXPECT_EQ(handler->name, "p:root");
}
