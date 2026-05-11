#include "jxx.util.IntStream.h"

namespace jxx::util {

IntStream::IntStream(jxx::Ptr<IntArray> backing) : data_(backing) {}

jxx::Ptr<IntStream> IntStream::of(jxx::Ptr<IntArray> backing) {
    return JXX_NEW<IntStream>(backing);
}

jxx::Ptr<IntArray> IntStream::toArray() const {
    // Return the backing array directly (minimal).
    // If you want defensive copy, tell me and I’ll adjust.
    return data_;
}

jxx::lang::jint IntStream::size() const {
    return data_ ? (jxx::lang::jint)data_->length : 0;
}

} // namespace jxx::util::stream