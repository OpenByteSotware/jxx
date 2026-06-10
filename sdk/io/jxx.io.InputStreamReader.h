#pragma once
#include <string>

#include "lang/jxx_types.h"
#include "lang/jxx.lang.buildin_array.h"
#include "jxx.io.Reader.h"
#include "jxx.io.InputStream.h"

namespace jxx::lang {
    class Charset;
	class String;
}
namespace jxx::io {

class InputStreamReader : public Reader {
public:
    explicit InputStreamReader(const jxx::Ptr<InputStream> in);
    InputStreamReader(const jxx::Ptr<InputStream> in, const jxx::Ptr<jxx::lang::Charset> cs);

    jxx::lang::jint read() override;
    jxx::lang::jint read(const jxx::lang::CharArray cbuf, jxx::lang::jint off, jxx::lang::jint len) override;

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
