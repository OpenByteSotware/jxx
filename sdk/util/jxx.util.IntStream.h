#pragma once

#include "lang/jxx_types.h"
#include "lang/jxx.lang.buildin_array.h"
#include "lang/jxx.lang.Object.h"
#include "io/jxx.io.SerializableI.h"

namespace jxx::util {

/**
 * Minimal java.util.stream.IntStream for String.chars()/codePoints().
 * Not a full Stream implementation—just enough for parity plumbing:
 *   - toArray()
 *   - size()
 */
class IntStream final : public jxx::lang::Object, public jxx::io::SerializableI {
public:
    IntStream() = default;
    explicit IntStream(jxx::lang::IntArray backing);

    static jxx::Ptr<IntStream> of(jxx::lang::IntArray backing);

    // Java: int[] toArray()
    jxx::lang::IntArray toArray() const;

    // convenience
    jxx::lang::jint size() const;

    virtual void writeObject(const jxx::Ptr<jxx::io::ObjectOutputStream>& out) override;
    virtual void readObject(const jxx::Ptr<jxx::io::ObjectInputStream>& in) override;
    virtual void readObjectNoData() override;

private:
    jxx::lang::IntArray data_;
};

}
