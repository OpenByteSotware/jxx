#include "io/jxx.io.SequenceInputStream.h"

#include "io/jxx.io.IOHelper.h"
#include "lang/jxx.lang.NullPointerException.h"

namespace jxx::io {

SequenceInputStream::SequenceInputStream(
    const ::jxx::Ptr<::jxx::util::Enumeration<InputStream>>& streams)
    : Super() {
    if (streams == nullptr) throw ::jxx::lang::NullPointerException();
    while (streams->hasMoreElements()) {
        auto stream = streams->nextElement();
        if (stream == nullptr) throw ::jxx::lang::NullPointerException();
        streams_.push_back(stream);
    }
}

SequenceInputStream::SequenceInputStream(
    const ::jxx::Ptr<InputStream>& first,
    const ::jxx::Ptr<InputStream>& second)
    : Super() {
    if (first == nullptr || second == nullptr) {
        throw ::jxx::lang::NullPointerException();
    }
    streams_.push_back(first);
    streams_.push_back(second);
}

SequenceInputStream::~SequenceInputStream() = default;

void SequenceInputStream::advance_() {
    if (index_ < streams_.size()) {
        streams_[index_]->close();
        ++index_;
    }
}

::jxx::lang::jint SequenceInputStream::read() {
    while (!closed_ && index_ < streams_.size()) {
        const auto value = streams_[index_]->read();
        if (value >= 0) return value;
        advance_();
    }
    return -1;
}

::jxx::lang::jint SequenceInputStream::read(
    const ::jxx::lang::ByteArray& buffer,
    ::jxx::lang::jint offset,
    ::jxx::lang::jint length) {
    IOHelper::checkBounds(buffer, offset, length);
    if (length == 0) return 0;
    while (!closed_ && index_ < streams_.size()) {
        const auto count = streams_[index_]->read(buffer, offset, length);
        if (count >= 0) return count;
        advance_();
    }
    return -1;
}

::jxx::lang::jint SequenceInputStream::available() {
    return !closed_ && index_ < streams_.size()
        ? streams_[index_]->available()
        : 0;
}

void SequenceInputStream::close() {
    if (closed_) return;
    closed_ = true;
    while (index_ < streams_.size()) advance_();
    streams_.clear();
}

} // namespace jxx::io
