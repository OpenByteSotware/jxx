#include "lang/jxx.lang.buildin_array.h"
#include "jxx.util.IntStream.h"

namespace jxx::util {

IntStream::IntStream(jxx::lang::IntArray backing) : data_(backing) {}

jxx::Ptr<IntStream> IntStream::of(jxx::lang::IntArray backing) {
    return jxx::NEW<IntStream>(backing);
}

jxx::lang::IntArray IntStream::toArray() const {
    // Return the backing array directly (minimal).
    // If you want defensive copy, tell me and I’ll adjust.
    return data_;
}

jxx::lang::jint IntStream::size() const {
    return data_ ? (jxx::lang::jint)data_->length : 0;
}

void IntStream::writeObject(jxx::Ptr<jxx::io::ObjectOutputStream> out)
{

}

void IntStream::readObject(jxx::Ptr<jxx::io::ObjectInputStream> in) {

}
void IntStream::readObjectNoData() {

}

} // namespace jxx::util::stream