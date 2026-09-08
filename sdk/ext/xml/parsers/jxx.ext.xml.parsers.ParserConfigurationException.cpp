#include "ext/xml/parsers/jxx.ext.xml.parsers.ParserConfigurationException.h"
#include "lang/jxx.lang.String.h"
namespace jxx::ext::xml::parsers
{
	ParserConfigurationException::ParserConfigurationException() = default; ParserConfigurationException::ParserConfigurationException(const jxx::Ptr<jxx::lang::String>& m) :jxx::lang::Exception(m)
	{
	} const char* ParserConfigurationException::typeName()const noexcept
	{
		return "ParserConfigurationException";
	} jxx::Ptr<jxx::lang::Object> ParserConfigurationException::cloneImpl()const
	{
		return jxx::CAST<jxx::lang::Object>(jxx::NEW<ParserConfigurationException>(*this));
	}
}
