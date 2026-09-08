#pragma once
#include "lang/jxx.lang.Exception.h"
namespace jxx::ext::xml::parsers
{
	class ParserConfigurationException : public jxx::lang::Exception
	{
	public: ParserConfigurationException(); 
	explicit ParserConfigurationException(const jxx::Ptr<jxx::lang::String>& message);
	protected: const char* typeName() const noexcept override; jxx::Ptr<jxx::lang::Object> cloneImpl() const override;
	};
}
