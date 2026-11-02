#include "nio/charset/jxx.nio.charset.ClassInitialization.h"
#include <mutex>
#include "nio/charset/jxx.nio.charset.Charset.h"
#include "nio/charset/jxx.nio.charset.CharsetDecoder.h"
#include "nio/charset/jxx.nio.charset.CharsetEncoder.h"
#include "nio/charset/jxx.nio.charset.CoderResult.h"
#include "nio/charset/jxx.nio.charset.CodingErrorAction.h"
#include "nio/charset/jxx.nio.charset.StandardCharsets.h"
namespace jxx::nio::charset {
void initializeClasses()
{
    static std::once_flag flag;
    std::call_once(flag, [] {
        (void)Charset::Class();
        (void)CharsetDecoder::Class();
        (void)CharsetEncoder::Class();
        (void)CoderResult::Class();
        (void)CodingErrorAction::Class();
        (void)StandardCharsets::Class();
    });
}

void initialize()
{
    initializeClasses();
}

} // namespace jxx::nio::charset
