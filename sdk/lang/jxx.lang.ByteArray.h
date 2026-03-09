#pragma once
#ifndef __JXX_BYTEARRAY__
#define __JXX_BYTEARRAY__

#include <iostream>
#include <vector>
#include <cstddef> // std::byte, std::to_integer
#include <stdexcept>

// ---------- Optional: demangle for GCC/Clang ----------
#if defined(__GNUG__) || defined(__clang__)
#include <cxxabi.h>
#include <cstdlib>
#endif

using namespace std;

// we really need to simulate byte type like java (signed)
// so try and define it that way
#if defined(__cplusplus) && __cplusplus >= 201703L
#include <cstddef>  // for std::byte
// Define our own alias only if std::byte is not being used
namespace compat {
    using byte = char;
}
#else
typedef char byte;
#endif

namespace jxx::lang {

    using ByteArray = std::vector<byte>; 
}
#endif

