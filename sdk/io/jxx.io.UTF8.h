
#pragma once
#include <string>
#include <vector>
#include <cstdint>
#include "lang/jxx.lang.JxxArray.h"
namespace jxx { namespace io {
class UTF8 {
public:
	static jxx::lang::ByteArray encode(const std::u16string& s);
	static std::u16string decode(const jxx::lang::ByteArray& bytes); };
}}
