#pragma once

namespace jxx::io {
    class Closeable {
    public:
        virtual ~Closeable() = default;

        virtual void close() = 0;
    };
}