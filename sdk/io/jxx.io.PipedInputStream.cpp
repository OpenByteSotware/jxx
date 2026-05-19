#include "jxx.io.PipedInputStream.h"
#include "jxx.io.PipedOutputStream.h"

namespace jxx::io {

static constexpr jxx::lang::jint DEFAULT_PIPE_SIZE = 1024;

void PipedInputStream::initPipe_(jxx::lang::jint pipeSize) {
    if (pipeSize <= 0) throw jxx::lang::IllegalArgumentException(JXX_NEW<jxx::lang::String>("pipeSize <= 0"));
    buffer_.assign((std::size_t)pipeSize, (jxx::lang::jbyte)0);
    in_ = -1;
    out_ = 0;
}

PipedInputStream::PipedInputStream() { initPipe_(DEFAULT_PIPE_SIZE); }
PipedInputStream::PipedInputStream(jxx::Ptr<PipedOutputStream> src) : PipedInputStream() { connect(src); }
PipedInputStream::PipedInputStream(jxx::lang::jint pipeSize) { initPipe_(pipeSize); }
PipedInputStream::PipedInputStream(jxx::Ptr<PipedOutputStream> src, jxx::lang::jint pipeSize) {
    initPipe_(pipeSize);
    connect(src);
}

void PipedInputStream::connect(jxx::Ptr<PipedOutputStream> src) {
    if (!src) throw jxx::lang::NullPointerException(JXX_NEW<jxx::lang::String>("src"));
    src->connect(std::static_pointer_cast<PipedInputStream>(this->thisPtr));
}

void PipedInputStream::receive_(jxx::lang::jint b) {
    if (!connected_) throw IOException(JXX_NEW<jxx::lang::String>("Pipe not connected"));
    if (closedByReader_) throw IOException(JXX_NEW<jxx::lang::String>("Pipe closed"));

    std::unique_lock<std::recursive_mutex> lk(mutex_);

    while (in_ == out_) {
        if (closedByReader_) throw IOException(JXX_NEW<jxx::lang::String>("Pipe closed"));
        notFull_.wait(lk);
    }

    if (in_ < 0) {
        in_ = 0;
        out_ = 0;
    }

    buffer_[(std::size_t)in_] = (jxx::lang::jbyte)(b & 0xFF);
    in_ = (in_ + 1) % (jxx::lang::jint)buffer_.size();

    notEmpty_.notify_all();
}

void PipedInputStream::receive_(jxx::Ptr<ByteArray> b, jxx::lang::jint off, jxx::lang::jint len) {
    InputStream::checkBounds_(b, off, len);
    for (jxx::lang::jint i = 0; i < len; ++i) {
        receive_(((jxx::lang::jint)(*b)[off + i]) & 0xFF);
    }
}

void PipedInputStream::receivedLast_() {
    std::lock_guard<std::recursive_mutex> lk(mutex_);
    closedByWriter_ = true;
    notEmpty_.notify_all();
}

jxx::lang::jint PipedInputStream::read() {
    std::unique_lock<std::recursive_mutex> lk(mutex_);

    if (!connected_) throw IOException(JXX_NEW<jxx::lang::String>("Pipe not connected"));
    if (closedByReader_) throw IOException(JXX_NEW<jxx::lang::String>("Pipe closed"));

    while (in_ < 0) {
        if (closedByWriter_) return -1;
        notEmpty_.wait(lk);
    }

    jxx::lang::jint ret = ((jxx::lang::jint)buffer_[(std::size_t)out_]) & 0xFF;
    out_ = (out_ + 1) % (jxx::lang::jint)buffer_.size();
    if (out_ == in_) in_ = -1;

    notFull_.notify_all();
    return ret;
}

jxx::lang::jint PipedInputStream::read(jxx::Ptr<ByteArray> b, jxx::lang::jint off, jxx::lang::jint len) {
    InputStream::checkBounds_(b, off, len);
    if (len == 0) return 0;

    jxx::lang::jint first = read();
    if (first < 0) return -1;
    (*b)[off] = (jxx::lang::jbyte)(first & 0xFF);

    jxx::lang::jint i = 1;
    for (; i < len; ++i) {
        std::lock_guard<std::recursive_mutex> lk(mutex_);
        if (in_ < 0) break;
        jxx::lang::jint v = ((jxx::lang::jint)buffer_[(std::size_t)out_]) & 0xFF;
        out_ = (out_ + 1) % (jxx::lang::jint)buffer_.size();
        if (out_ == in_) in_ = -1;
        (*b)[off + i] = (jxx::lang::jbyte)(v & 0xFF);
        notFull_.notify_all();
    }

    return i;
}

jxx::lang::jint PipedInputStream::available() {
    std::lock_guard<std::recursive_mutex> lk(mutex_);
    if (in_ < 0) return 0;
    if (in_ == out_) return (jxx::lang::jint)buffer_.size();
    if (in_ > out_) return in_ - out_;
    return (jxx::lang::jint)buffer_.size() - out_ + in_;
}

void PipedInputStream::close() {
    std::lock_guard<std::recursive_mutex> lk(mutex_);
    closedByReader_ = true;
    in_ = -1;
    notFull_.notify_all();
    notEmpty_.notify_all();
}

jxx::lang::jbool PipedInputStream::markSupported() { return false; }

} // namespace jxx::io
