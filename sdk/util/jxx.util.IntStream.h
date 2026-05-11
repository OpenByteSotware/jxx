#pragma once

#include "lang/jxx_types.h"
#include "lang/jxx.lang.buildin_array.h"
#include "lang/jxx.lang.Object.h"
#include "io/jxx.io.Serializable.h"

namespace jxx::util {

/**
 * Minimal java.util.stream.IntStream for String.chars()/codePoints().
 * Not a full Stream implementation—just enough for parity plumbing:
 *   - toArray()
 *   - size()
 */
class IntStream final : public jxx::lang::Object, public jxx::io::Serializable {
public:
    IntStream() = default;
    explicit IntStream(jxx::Ptr<IntArray> backing);

    static jxx::Ptr<IntStream> of(jxx::Ptr<IntArray> backing);

    // Java: int[] toArray()
    jxx::Ptr<IntArray> toArray() const;

    // convenience
    jxx::lang::jint size() const;

private:
    jxx::Ptr<IntArray> data_;
};

}
