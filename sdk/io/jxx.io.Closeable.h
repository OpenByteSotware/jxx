#pragma once
namespace jxx::io {

    /** Java 8: java.io.Closeable (interface) */
    struct Closeable {
        virtual ~Closeable() = default;
        virtual void close() = 0;
    };

} // namespace jxx::io
