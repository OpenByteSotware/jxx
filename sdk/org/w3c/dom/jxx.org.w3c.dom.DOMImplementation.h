#pragma once
#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.String.h"
#include "lang/jxx_types.h"

namespace jxx::org::w3c::dom
{
	class Document;
	class DOMImplementation : public jxx::lang::InterfaceBase<DOMImplementation>
	{
	public: ~DOMImplementation() override = default; 
		  virtual jxx::lang::jbool hasFeature(const jxx::Ptr<jxx::lang::String>& feature,
			  const jxx::Ptr<jxx::lang::String>& version) const = 0; 
		  virtual jxx::Ptr<Document> createDocument(const jxx::Ptr<jxx::lang::String>& namespaceURI,
			  const jxx::Ptr<jxx::lang::String>& qualifiedName) = 0;
	};
} // namespace jxx::org::w3c::dom
