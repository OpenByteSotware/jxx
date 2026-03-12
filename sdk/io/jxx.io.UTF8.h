
#pragma once
#include <string>
#include <vector>
#include <cstdint>
#include "lang/jxx.lang.internal.h"
namespace jxx { namespace io {
class UTF8 {
public:
	static shared_ptr<ByteArray> encode(const std::u16string& s);
	static std::u16string decode(const ByteArray& bytes); };
}}
