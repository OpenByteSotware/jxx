#pragma once
#include <unordered_map>
#include "ext/xml/parsers/jxx.ext.xml.parsers.DocumentBuilderFactory.h"
namespace jxx::ext::xml::parsers::internal
{
	class PugiDocumentBuilderFactory final :
		public jxx::lang::ClassBase<PugiDocumentBuilderFactory, DocumentBuilderFactory>
	{
	public: jxx::Ptr<DocumentBuilder> newDocumentBuilder() override; 
	void setAttribute(const jxx::Ptr<jxx::lang::String>& name, 
	const jxx::Ptr<jxx::lang::Object>& value) override;
	jxx::Ptr<jxx::lang::Object> getAttribute(const jxx::Ptr<jxx::lang::String>& name) override;
	void setFeature(const jxx::Ptr<jxx::lang::String>& name, jxx::lang::jbool value) override; 
	jxx::lang::jbool getFeature(const jxx::Ptr<jxx::lang::String>& name) override; 
	private:
		std::unordered_map<std::string, jxx::Ptr<jxx::lang::Object>> attributes_;
		std::unordered_map<std::string, jxx::lang::jbool> features_;
	};
}
