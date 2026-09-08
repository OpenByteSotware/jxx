#pragma once
#include "lang/jxx.lang.ClassInfo.h"
namespace jxx::lang { class String; }
namespace jxx::org::xml::sax { class InputSource; class SAXParseException; class ErrorHandler : public jxx::lang::InterfaceBase<ErrorHandler> { public: ~ErrorHandler() override=default; virtual void warning(const jxx::Ptr<SAXParseException>& exception)=0; virtual void error(const jxx::Ptr<SAXParseException>& exception)=0; virtual void fatalError(const jxx::Ptr<SAXParseException>& exception)=0; }; }
