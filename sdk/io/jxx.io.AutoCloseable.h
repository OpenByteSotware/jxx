#pragma once
namespace jxx::io { struct AutoCloseable { virtual ~AutoCloseable() = default; virtual void close() = 0; }; }