#pragma once
#include "jxx.io.AutoCloseable.h"
namespace jxx::io { struct Closeable : public AutoCloseable { virtual ~Closeable() = default; }; }