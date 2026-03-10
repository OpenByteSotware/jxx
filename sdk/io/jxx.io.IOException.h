
#pragma once
#include <stdexcept>
namespace jxx { namespace io {
struct IOException : public std::runtime_error { using std::runtime_error::runtime_error; };
}} 
