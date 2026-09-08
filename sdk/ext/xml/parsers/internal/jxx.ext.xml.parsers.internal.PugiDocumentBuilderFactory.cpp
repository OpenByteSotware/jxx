#include "ext/xml/parsers/internal/jxx.ext.xml.parsers.internal.PugiDocumentBuilderFactory.h"
#include "ext/xml/parsers/internal/jxx.ext.xml.parsers.internal.PugiDocumentBuilder.h"
#include "ext/xml/parsers/jxx.ext.xml.parsers.ParserConfigurationException.h"
#include "lang/jxx.lang.IllegalArgumentException.h"
#include "lang/jxx.lang.String.h"
namespace jxx::ext::xml::parsers::internal
{
	jxx::Ptr<DocumentBuilder> PugiDocumentBuilderFactory::newDocumentBuilder()
	{
		if (validating_ || xinclude_)throw ParserConfigurationException(jxx::NEW<jxx::lang::String>("Validation and XInclude are not supported")); return jxx::NEW<PugiDocumentBuilder>(namespaceAware_, validating_, ignoreWhitespace_, expandEntities_, ignoreComments_, coalescing_);
	} void PugiDocumentBuilderFactory::setAttribute(const jxx::Ptr<jxx::lang::String>& n, const jxx::Ptr<jxx::lang::Object>& v)
	{
		attributes_[n->utf8()] = v;
	} jxx::Ptr<jxx::lang::Object> PugiDocumentBuilderFactory::getAttribute(const jxx::Ptr<jxx::lang::String>& n)
	{
		auto i = attributes_.find(n->utf8()); if (i == attributes_.end())throw jxx::lang::IllegalArgumentException(jxx::NEW<jxx::lang::String>("Unknown attribute")); return i->second;
	} void PugiDocumentBuilderFactory::setFeature(const jxx::Ptr<jxx::lang::String>& n, jxx::lang::jbool v)
	{
		features_[n->utf8()] = v;
	} jxx::lang::jbool PugiDocumentBuilderFactory::getFeature(const jxx::Ptr<jxx::lang::String>& n)
	{
		auto i = features_.find(n->utf8()); if (i == features_.end())throw ParserConfigurationException(jxx::NEW<jxx::lang::String>("Unknown feature")); return i->second;
	}
}
