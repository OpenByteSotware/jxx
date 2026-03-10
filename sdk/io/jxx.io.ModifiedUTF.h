
#pragma once
#include <string>
#include "lang/jxx.lang.internal.h"
#include "io/jxx.io.IOException.h"
namespace jxx { namespace io {
class ModifiedUTF { static ByteArray encode(const std::u16string& s); static std::u16string decode(const ByteArray& bytes); };
}}
