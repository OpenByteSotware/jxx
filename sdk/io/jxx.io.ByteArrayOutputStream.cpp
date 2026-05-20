#include "jxx.io.IOHelper.h"
#include "lang/jxx.lang.IllegalArgumentException.h"
#include "lang/jxx.lang.NullPointerException.h"
#include "jxx.io.ByteArrayOutputStream.h"

namespace jxx::io {

static constexpr std::uint32_t DEFAULT_BAOS_SIZE = 32;

ByteArrayOutputStream::ByteArrayOutputStream()
    : buf_(JXX_NEW<ByteArray>(DEFAULT_BAOS_SIZE)), count_(0) {}

ByteArrayOutputStream::ByteArrayOutputStream(jxx::lang::jint size)
    : buf_(nullptr), count_(0) {
    if (size < 0) throw jxx::lang::IllegalArgumentException(JXX_NEW<jxx::lang::String>("negative size"));
    buf_ = JXX_NEW<ByteArray>((std::uint32_t)size);
}

void ByteArrayOutputStream::ensureCapacity_(jxx::lang::jint minCapacity) {
    if (!buf_) buf_ = JXX_NEW<ByteArray>(DEFAULT_BAOS_SIZE);
    if (minCapacity <= (jxx::lang::jint)buf_->length) return;

    std::uint32_t oldCap = buf_->length;
    std::uint32_t newCap = oldCap ? oldCap * 2 : DEFAULT_BAOS_SIZE;
    if ((jxx::lang::jint)newCap < minCapacity) newCap = (std::uint32_t)minCapacity;

    auto nb = JXX_NEW<ByteArray>(newCap);
    for (std::uint32_t i = 0; i < oldCap; ++i) (*nb)[(jxx::lang::jint)i] = (*buf_)[(jxx::lang::jint)i];
    buf_ = nb;
}

void ByteArrayOutputStream::write(jxx::lang::jint b) {
    ensureCapacity_(count_ + 1);
    (*buf_)[count_++] = (jxx::lang::jbyte)(b & 0xFF);
}

void ByteArrayOutputStream::write(jxx::Ptr<ByteArray> b, jxx::lang::jint off, jxx::lang::jint len) {
    IOHelper::checkBounds_(b, off, len);
    ensureCapacity_(count_ + len);
    for (jxx::lang::jint i = 0; i < len; ++i) (*buf_)[count_ + i] = (*b)[off + i];
    count_ += len;
}

void ByteArrayOutputStream::reset() { count_ = 0; }

jxx::lang::jint ByteArrayOutputStream::size() const { return count_; }

jxx::Ptr<ByteArray> ByteArrayOutputStream::toByteArray() const {
    auto out = JXX_NEW<ByteArray>((std::uint32_t)count_);
    for (jxx::lang::jint i = 0; i < count_; ++i) (*out)[i] = (*buf_)[i];
    return out;
}

void ByteArrayOutputStream::writeTo(jxx::Ptr<OutputStream> out) const {
    if (!out) throw jxx::lang::NullPointerException(JXX_NEW<jxx::lang::String>("out"));
    out->write(buf_, 0, count_);
}

void ByteArrayOutputStream::close() {}

} // namespace jxx::io
