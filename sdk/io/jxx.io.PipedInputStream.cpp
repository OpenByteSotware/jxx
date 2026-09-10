#include "io/jxx.io.PipedInputStream.h"

#include <algorithm>
#include <cstdint>
#include <memory>

#include "io/jxx.io.IOException.h"
#include "io/jxx.io.IOHelper.h"
#include "io/jxx.io.PipeState.h"
#include "io/jxx.io.PipedOutputStream.h"
#include "lang/jxx.lang.IllegalArgumentException.h"
#include "lang/jxx.lang.NullPointerException.h"

namespace jxx::io {

PipedInputStream::PipedInputStream()
    : PipedInputStream(PIPE_SIZE) {
}

PipedInputStream::PipedInputStream(::jxx::lang::jint pipeSize)
    : Super()
    , state_(std::make_shared<internal::PipeState>(
          validatePipeSize_(pipeSize))) {
}

PipedInputStream::PipedInputStream(
    const ::jxx::Ptr<PipedOutputStream>& source)
    : PipedInputStream(source, PIPE_SIZE) {
}

PipedInputStream::PipedInputStream(
    const ::jxx::Ptr<PipedOutputStream>& source,
    ::jxx::lang::jint pipeSize)
    : PipedInputStream(pipeSize) {
    connect(source);
}

PipedInputStream::~PipedInputStream() = default;

::jxx::lang::jint PipedInputStream::validatePipeSize_(
    ::jxx::lang::jint pipeSize) {
    if (pipeSize <= 0) {
        throw ::jxx::lang::IllegalArgumentException();
    }
    return pipeSize;
}

void PipedInputStream::attach_(
    const std::shared_ptr<internal::PipeState>& state) {
    if (!state) {
        throw ::jxx::lang::NullPointerException();
    }
    if (state_ != state) {
        throw IOException();
    }
}

void PipedInputStream::connect(
    const ::jxx::Ptr<PipedOutputStream>& source) {
    if (source == nullptr) {
        throw ::jxx::lang::NullPointerException();
    }
    source->connect(::jxx::CAST<PipedInputStream>(thisPtr()));
}

::jxx::lang::jint PipedInputStream::read() {
    auto state = state_;
    std::unique_lock<std::mutex> lock(state->mutex);

    if (!state->connected || state->inputClosed) {
        throw IOException();
    }

    state->readable.wait(lock, [&] {
        return state->count > 0 || state->outputClosed || state->inputClosed;
    });

    if (state->inputClosed) {
        throw IOException();
    }
    if (state->count == 0 && state->outputClosed) {
        return -1;
    }

    const auto value = static_cast<::jxx::lang::jint>(
        static_cast<unsigned char>(state->buffer[state->readPosition]));
    state->readPosition = (state->readPosition + 1) % state->buffer.size();
    --state->count;
    lock.unlock();
    state->writable.notify_all();
    return value;
}

::jxx::lang::jint PipedInputStream::read(
    const ::jxx::lang::ByteArray& buffer,
    ::jxx::lang::jint offset,
    ::jxx::lang::jint length) {
    IOHelper::checkBounds(buffer, offset, length);
    if (length == 0) return 0;

    const auto first = read();
    if (first < 0) return -1;
    (*buffer)[offset] = static_cast<::jxx::lang::jbyte>(first);

    ::jxx::lang::jint total = 1;
    auto state = state_;
    std::unique_lock<std::mutex> lock(state->mutex);
    while (total < length && state->count > 0) {
        (*buffer)[offset + total] = state->buffer[state->readPosition];
        state->readPosition = (state->readPosition + 1) % state->buffer.size();
        --state->count;
        ++total;
    }
    lock.unlock();
    state->writable.notify_all();
    return total;
}

::jxx::lang::jint PipedInputStream::available() {
    auto state = state_;
    std::lock_guard<std::mutex> lock(state->mutex);
    if (state->inputClosed) throw IOException();
    return static_cast<::jxx::lang::jint>(state->count);
}

void PipedInputStream::close() {
    auto state = state_;
    {
        std::lock_guard<std::mutex> lock(state->mutex);
        if (state->inputClosed) return;
        state->inputClosed = true;
    }
    state->readable.notify_all();
    state->writable.notify_all();
}

} // namespace jxx::io
