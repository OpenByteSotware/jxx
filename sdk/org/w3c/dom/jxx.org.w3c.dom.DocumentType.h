#pragma once
#include "lang/jxx.lang.ClassInfo.h"
#include "org/w3c/dom/jxx.org.w3c.dom.Node.h"
namespace jxx::org::w3c::dom { class NamedNodeMap; class DocumentType : public jxx::lang::InterfaceBase<DocumentType, Node> { public: ~DocumentType() override=default; virtual jxx::Ptr<jxx::lang::String> getName() const=0; virtual jxx::Ptr<NamedNodeMap> getEntities() const=0; virtual jxx::Ptr<NamedNodeMap> getNotations() const=0; virtual jxx::Ptr<jxx::lang::String> getPublicId() const=0; virtual jxx::Ptr<jxx::lang::String> getSystemId() const=0; virtual jxx::Ptr<jxx::lang::String> getInternalSubset() const=0; }; }
