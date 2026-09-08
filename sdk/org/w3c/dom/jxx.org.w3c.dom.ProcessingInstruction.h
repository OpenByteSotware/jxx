#pragma once
#include "lang/jxx.lang.ClassInfo.h"
#include "org/w3c/dom/jxx.org.w3c.dom.Node.h"
namespace jxx::org::w3c::dom { class ProcessingInstruction : public jxx::lang::InterfaceBase<ProcessingInstruction, Node> { public: ~ProcessingInstruction() override=default; virtual jxx::Ptr<jxx::lang::String> getTarget() const=0; virtual jxx::Ptr<jxx::lang::String> getData() const=0; virtual void setData(const jxx::Ptr<jxx::lang::String>& data)=0; }; }
