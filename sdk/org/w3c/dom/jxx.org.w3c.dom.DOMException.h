#pragma once
#include "lang/jxx.lang.RuntimeException.h"
namespace jxx::org::w3c::dom {
class DOMException : public jxx::lang::RuntimeException { public: static constexpr jxx::lang::jshort INDEX_SIZE_ERR=1, HIERARCHY_REQUEST_ERR=3, WRONG_DOCUMENT_ERR=4, NOT_FOUND_ERR=8, NOT_SUPPORTED_ERR=9, INVALID_CHARACTER_ERR=5; DOMException(jxx::lang::jshort code,const jxx::Ptr<jxx::lang::String>& message); jxx::lang::jshort code; protected: const char* typeName() const noexcept override; jxx::Ptr<jxx::lang::Object> cloneImpl() const override; };
}
