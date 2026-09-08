#pragma once
#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.String.h"
#include "lang/jxx_types.h"

namespace jxx::org::w3c::dom
{
	class Node;
	class NodeList : public jxx::lang::InterfaceBase<NodeList>
	{
	public: ~NodeList() override = default; virtual jxx::Ptr<Node> item(jxx::lang::jint index) const = 0; virtual jxx::lang::jint getLength() const = 0;
	};
} // namespace jxx::org::w3c::dom
