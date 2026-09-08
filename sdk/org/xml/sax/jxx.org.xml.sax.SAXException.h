#pragma once
#include "lang/jxx.lang.Exception.h"
namespace jxx::org::xml::sax { class SAXException : public jxx::lang::Exception { public: SAXException(); explicit SAXException(const jxx::Ptr<jxx::lang::String>& message); SAXException(const jxx::Ptr<jxx::lang::String>& message,const jxx::Ptr<jxx::lang::Throwable>& cause); protected: const char* typeName() const noexcept override; jxx::Ptr<jxx::lang::Object> cloneImpl() const override; }; }
