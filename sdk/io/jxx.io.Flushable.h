#pragma once

namespace jxx::io {

// Java 8: java.io.Flushable (interface)
class Flushable {
public:
    virtual ~Flushable() = default;
    virtual void flush() = 0;
};

} // namespace jxx::io
