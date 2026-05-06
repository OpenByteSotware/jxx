#pragma once
namespace jxx::io { struct Flushable { virtual ~Flushable() = default; virtual void flush() = 0; }; }