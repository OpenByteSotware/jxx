#include "io/jxx.io.PipedOutputStream.h"

#include <mutex>

#include "io/jxx.io.IOException.h"
#include "io/jxx.io.IOHelper.h"
#include "io/jxx.io.PipeState.h"
#include "io/jxx.io.PipedInputStream.h"
#include "lang/jxx.lang.NullPointerException.h"

namespace jxx::io {

PipedOutputStream::PipedOutputStream()
    : Super() {
}

PipedOutputStream::PipedOutputStream(
    const ::jxx::Ptr<PipedInputStream>& sink)
    : Super() {
    connect(sink);
}

PipedOutputStream::~PipedOutputStream() = default;

void PipedOutputStream::connect(
    const ::jxx::Ptr<PipedInputStream>& sink) {
    if (sink == nullptr) {
        throw ::jxx::lang::NullPointerException();
    }
    if (state_ != nullptr) {
        throw IOException();
    }

    auto state = sink->state_;
    std::lock_guard<std::mutex> lock(state->mutex);
    if (state->connected || state->inputClosed || state->outputClosed) {
        throw IOException();
    }
    state->connected = true;
    state_ = state;
}

void PipedOutputStream::write(::jxx::lang::jint value) {
    auto state = state_;
    if (!state) throw IOException();

    std::unique_lock<std::mutex> lock(state->mutex);
    if (!state->connected || state->outputClosed || state->inputClosed) {
        throw IOException();
    }

    state->writable.wait(lock, [&] {
        return state->count < state->buffer.size() ||
               state->inputClosed || state->outputClosed;
    });

    if (state->inputClosed || state->outputClosed) {
        throw IOException();
    }

    state->buffer[state->writePosition] =
        static_cast<::jxx::lang::jbyte>(value);
    state->writePosition = (state->writePosition + 1) % state->buffer.size();
    ++state->count;
    lock.unlock();
    state->readable.notify_all();
}

void PipedOutputStream::write(
    const ::jxx::lang::ByteArray& buffer,
    ::jxx::lang::jint offset,
    ::jxx::lang::jint length) {
    IOHelper::checkBounds(buffer, offset, length);
    for (::jxx::lang::jint index = 0; index < length; ++index) {
        write(static_cast<::jxx::lang::jint>(
            static_cast<unsigned char>((*buffer)[offset + index])));
    }
}

void PipedOutputStream::flush() {
    auto state = state_;
    if (!state) throw IOException();
    std::lock_guard<std::mutex> lock(state->mutex);
    if (state->outputClosed || state->inputClosed) throw IOException();
    state->readable.notify_all();
}

void PipedOutputStream::close() {
    auto state = state_;
    if (!state) return;
    {
        std::lock_guard<std::mutex> lock(state->mutex);
        if (state->outputClosed) return;
        state->outputClosed = true;
    }
    state->readable.notify_all();
    state->writable.notify_all();
}

} // namespace jxx::io
