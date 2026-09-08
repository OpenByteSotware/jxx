#include "ext/xml/parsers/jxx.ext.xml.parsers.DocumentBuilder.h"
#include "io/jxx.io.File.h"
#include "org/xml/sax/jxx.org.xml.sax.InputSource.h"
namespace jxx::ext::xml::parsers
{
	jxx::Ptr<jxx::org::w3c::dom::Document> DocumentBuilder::parse(const jxx::Ptr<jxx::io::InputStream>& i)
	{
		return parse(jxx::NEW<jxx::org::xml::sax::InputSource>(i));
	} jxx::Ptr<jxx::org::w3c::dom::Document> DocumentBuilder::parse(const jxx::Ptr<jxx::io::InputStream>& i, const jxx::Ptr<jxx::lang::String>& s)
	{
		auto x = jxx::NEW<jxx::org::xml::sax::InputSource>(i); x->setSystemId(s); return parse(x);
	} jxx::Ptr<jxx::org::w3c::dom::Document> DocumentBuilder::parse(const jxx::Ptr<jxx::io::File>& f)
	{
		return parse(f->getPath());
	} jxx::Ptr<jxx::org::w3c::dom::Document> DocumentBuilder::parse(const jxx::Ptr<jxx::lang::String>& u)
	{
		return parse(jxx::NEW<jxx::org::xml::sax::InputSource>(u));
	} void DocumentBuilder::reset()
	{
	}
}
