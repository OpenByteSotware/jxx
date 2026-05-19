#pragma once

#include "jxx.io.Reader.h"
#include "jxx.io.InputStream.h"

#include "lang/jxx.lang.Charset.h"
#include "lang/jxx.lang.String.h"
#include "lang/jxx.lang.NullPointerException.h"

#include <string>

namespace jxx::io {

class InputStreamReader : public Reader {
public:
    explicit InputStreamReader(jxx::Ptr<InputStream> in);
    InputStreamReader(jxx::Ptr<InputStream> in, jxx::Ptr<jxx::lang::Charset> cs);

    jxx::lang::jint read() override;
    jxx::lang::jint read(jxx::Ptr<CharArray> cbuf, jxx::lang::jint off, jxx::lang::jint len) override;

    jxx::lang::jbool ready() override;
    void close() override;

private:
    jxx::Ptr<InputStream> in_;
    jxx::Ptr<jxx::lang::Charset> cs_;

    std::u16string decoded_;
    std::size_t dpos_ = 0;

    jxx::lang::jbool refill_();
};

} // namespace jxx::io
