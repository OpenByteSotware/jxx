// Get all SDK, in the future can specify which ones to use
// but for now, it is all or nothing for ease of use
#pragma once

#include <cstdint>
#include <cstddef>
#include <functional>
#include <memory>
#include <mutex>
#include <sstream>
#include <string>
#include <typeinfo>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <type_traits>
#include <numeric>
#include <array>
#include <stdexcept>
#include <cstdio>
#include <condition_variable>
#include <chrono>
#include <iostream>

// ---------- Optional: demangle for GCC/Clang ----------
#if defined(__GNUG__) || defined(__clang__)
#include <cxxabi.h>
#include <cstdlib>

#endif

namespace jxx::lang {
	// -------------------- Primitive types --------------------
	using jbyte = std::int8_t;     // Java byte:  8-bit signed
	using jshort = std::int16_t;    // Java short: 16-bit signed
	using jchar = char16_t;        // Java char:  16-bit UTF-16 code unit (unsigned in Java, but char16_t fits semantics)
	using jint = std::int32_t;    // Java int:   32-bit signed
	using jlong = std::int64_t;    // Java long:  64-bit signed
	using jfloat = float;           // Java float: IEEE-754 binary32 on typical targets
	using jdouble = double;          // Java double: IEEE-754 binary64 on typical targets
	using jbool = bool;            // Java boolean: logical true/false
}
