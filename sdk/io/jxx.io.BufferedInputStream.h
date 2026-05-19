#pragma once

#include "lang/jxx_types.h"
#include "lang/jxx.lang.buildin_array.h"
#include "jxx.io.FilterInputStream.h"

namespace jxx::io {

    class BufferedInputStream : public FilterInputStream {
    public:
        explicit BufferedInputStream(jxx::Ptr<InputStream> in, jxx::lang::jint size = 8192);

        jxx::lang::jint read() override;

    private:
        jxx::Ptr<ByteArray> buf_;
        jxx::lang::jint pos_ = 0;
        jxx::lang::jint count_ = 0;
    };

} // namespace jxx::io