#include "lang/jxx.lang.buildin_array.h"
#include "util/jxx.util.IntStream.h"
#include "io/jxx.io.ObjectInputStream.h"
#include "io/jxx.io.ObjectOutputStream.h"
#include "lang/jxx.lang.NullPointerException.h"

namespace jxx::util {

IntStream::IntStream(const ::jxx::lang::IntArray& backing) : data_(backing) {}

::jxx::Ptr<IntStream> IntStream::of(const ::jxx::lang::IntArray& backing) {
    return ::jxx::NEW<IntStream>(backing);
}

::jxx::lang::IntArray IntStream::toArray() const {
    // Return the backing array directly (minimal).
    // If you want defensive copy, tell me and I’ll adjust.
    return data_;
}

::jxx::lang::jint IntStream::size() const {
    return data_ ? (::jxx::lang::jint)data_->length : 0;
}

void IntStream::writeObject(const ::jxx::Ptr<::jxx::io::ObjectOutputStream>& out)
{
    if (out == nullptr) throw ::jxx::lang::NullPointerException();
    out->writeObject(data_);
}

void IntStream::readObject(const ::jxx::Ptr<::jxx::io::ObjectInputStream>& in) {
    if (in == nullptr) throw ::jxx::lang::NullPointerException();
    data_ = ::jxx::CAST<::jxx::lang::IntArrayType>(in->readObject());
}
void IntStream::readObjectNoData() { data_ = nullptr; }

} // namespace jxx::util