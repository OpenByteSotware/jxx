#include "lang/jxx.lang.String.h"
#include "lang/jxx.lang.NullPointerException.h"
#include "jxx.io.PipedInputStream.h"
#include "jxx.io.PipedOutputStream.h"

namespace jxx::io {

PipedOutputStream::PipedOutputStream() = default;
PipedOutputStream::PipedOutputStream(jxx::Ptr<PipedInputStream> snk) { connect(std::move(snk)); }

void PipedOutputStream::connect(jxx::Ptr<PipedInputStream> snk) {
    if (!snk) throw jxx::lang::NullPointerException(jxx::NEW<jxx::lang::String>("snk"));
    if (connected_) throw IOException(jxx::NEW<jxx::lang::String>("Already connected"));

    sink_ = std::move(snk);

    {
        std::lock_guard<std::recursive_mutex> lk(sink_->mutex_);
        if (sink_->connected_) throw IOException(jxx::NEW<jxx::lang::String>("Already connected"));
        sink_->connected_ = true;
    }

    connected_ = true;
}

void PipedOutputStream::write(jxx::lang::jint b) {
    if (closed_) throw IOException(jxx::NEW<jxx::lang::String>("Pipe closed"));
    if (!sink_ || !connected_) throw IOException(jxx::NEW<jxx::lang::String>("Pipe not connected"));
    sink_->receive_(b);
}

void PipedOutputStream::write(const jxx::lang::ByteArray b, jxx::lang::jint off, jxx::lang::jint len) {
    if (closed_) throw IOException(jxx::NEW<jxx::lang::String>("Pipe closed"));
    if (!sink_ || !connected_) throw IOException(jxx::NEW<jxx::lang::String>("Pipe not connected"));
    sink_->receive_(b, off, len);
}

void PipedOutputStream::flush() {
    if (sink_) {
        std::lock_guard<std::recursive_mutex> lk(sink_->mutex_);
        sink_->notEmpty_.notify_all();
    }
}

void PipedOutputStream::close() {
    closed_ = true;
    if (sink_) sink_->receivedLast_();
}

} // namespace jxx::io
