#pragma once

#include "jxx.io.Reader.h"
#include "jxx.io.InputStream.h"

#include "jxx.lang.Charset.h"
#include "jxx.lang.String.h"
#include "jxx.lang.NullPointerException.h"

#include <string>

namespace jxx::io {

class InputStreamReader : public Reader {
public:
    explicit InputStreamReader(jxx::Ptr<InputStream> in);
    InputStreamReader(jxx::Ptr<InputStream> in, jxx::Ptr<jxx::lang::Charset> cs);

    jint read() override;
    jint read(jxx::Ptr<CharArray> cbuf, jint off, jint len) override;

    jbool ready() override;
    void close() override;

private:
    jxx::Ptr<InputStream> in_;
    jxx::Ptr<jxx::lang::Charset> cs_;

    std::u16string decoded_;
    std::size_t dpos_ = 0;

    jbool refill_();
};

} // namespace jxx::io
