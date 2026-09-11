#include <gtest/gtest.h>
#include <string>
#include <vector>
#include "io/jxx.io.StringReader.h"
#include "lang/jxx.lang.String.h"
#include "org/xml/sax/helpers/jxx.org.xml.sax.helpers.DefaultHandler.h"
#include "org/xml/sax/helpers/jxx.org.xml.sax.helpers.XMLFilterImpl.h"
#include "org/xml/sax/helpers/jxx.org.xml.sax.helpers.XMLReaderFactory.h"
#include "org/xml/sax/jxx.org.xml.sax.InputSource.h"
namespace
{
	class Sink final :public ::jxx::org::xml::sax::helpers::DefaultHandler
	{
	public:std::vector<std::string> names;
		  void startElement(const ::jxx::Ptr<::jxx::lang::String>&,
			  const ::jxx::Ptr<::jxx::lang::String>& l, const ::jxx::Ptr<::jxx::lang::String>&,
			  const ::jxx::Ptr<::jxx::org::xml::sax::Attributes>&)override
		  {
			  names.push_back(l->utf8());
		  }
	};
}
TEST(XmlSaxFilterStage, ForwardsParentEvents)
{
	auto parent = ::jxx::org::xml::sax::helpers::XMLReaderFactory::createXMLReader();
	auto filter = ::jxx::NEW<::jxx::org::xml::sax::helpers::XMLFilterImpl>(parent);
	auto sink = ::jxx::NEW<Sink>(); filter->setContentHandler(sink);
	auto source =
		::jxx::NEW<::jxx::org::xml::sax::InputSource>(::jxx::NEW<::jxx::io::StringReader>(::jxx::NEW<::jxx::lang::String>("<root><item/></root>")));
	filter->parse(source); 
	ASSERT_EQ(sink->names.size(), 2U); 
	EXPECT_EQ(sink->names[0], "root"); 
	EXPECT_EQ(sink->names[1], "item");
}
