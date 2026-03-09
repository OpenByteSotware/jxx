// Get all SDK, in the future can specify which ones to use
// but for now, it is all or nothing for ease of use
#pragma once
#ifndef _JXX_CONST_HEADER_
#define _JXX_CONST_HEADER_
#include <cstddef>   // std::byte, std::to_integer

#if defined(_WIN32)
#ifndef byte
typedef char byte;
#endif
#endif
namespace jxx {
	constexpr std::size_t NPOS = static_cast<std::size_t>(-1);
}

#endif
