#pragma once
#include "lang/jxx.lang.Error.h"
namespace jxx::ext::xml::parsers { class FactoryConfigurationError : public jxx::lang::Error { public: FactoryConfigurationError(); explicit FactoryConfigurationError(const jxx::Ptr<jxx::lang::String>& message); protected: const char* typeName()const noexcept override; jxx::Ptr<jxx::lang::Object> cloneImpl()const override; }; }
