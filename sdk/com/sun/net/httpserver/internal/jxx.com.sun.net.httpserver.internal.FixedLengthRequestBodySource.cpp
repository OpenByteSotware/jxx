#include "com/sun/net/httpserver/internal/jxx.com.sun.net.httpserver.internal.FixedLengthRequestBodySource.h"

#include "io/jxx.io.IOException.h"
#include "io/jxx.io.InputStream.h"
#include "lang/jxx.lang.Exceptions.h"

#include <algorithm>
#include <limits>

namespace jxx::com::sun::net::httpserver::internal {

FixedLengthRequestBodySource::FixedLengthRequestBodySource(
    const ::jxx::Ptr<::jxx::io::InputStream>& input,
    const ::jxx::lang::ByteArray& prefix,
    ::jxx::lang::jint prefixOffset,
    ::jxx::lang::jint prefixLength,
    ::jxx::lang::jlong contentLength)
    : Super(), input_(input), prefix_(prefix), prefixPosition_(prefixOffset),
      prefixLimit_(prefixOffset + prefixLength), remaining_(contentLength)
{
    if (input_ == nullptr || prefix_ == nullptr) throw ::jxx::lang::NullPointerException();
    if (prefixOffset < 0 || prefixLength < 0 || prefixOffset > prefix_->length - prefixLength)
        throw ::jxx::lang::IndexOutOfBoundsException();
    if (contentLength < 0 || prefixLength > contentLength)
        throw ::jxx::lang::IllegalArgumentException();
}

::jxx::lang::jint FixedLengthRequestBodySource::read(
    const ::jxx::lang::ByteArray& buffer,
    ::jxx::lang::jint offset,
    ::jxx::lang::jint length)
{
    if (buffer == nullptr) throw ::jxx::lang::NullPointerException();
    if (offset < 0 || length < 0 || offset > buffer->length - length)
        throw ::jxx::lang::IndexOutOfBoundsException();
    if (length == 0) return 0;
    if (closed_ || remaining_ == 0) return -1;

    const auto maximum = static_cast<::jxx::lang::jint>(std::min<::jxx::lang::jlong>(remaining_, length));
    ::jxx::lang::jint copied = 0;
    while (copied < maximum && prefixPosition_ < prefixLimit_) {
        (*buffer)[offset + copied++] = (*prefix_)[prefixPosition_++];
    }
    if (copied < maximum) {
        const auto count = input_->read(buffer, offset + copied, maximum - copied);
        if (count < 0) throw ::jxx::io::IOException("unexpected end of fixed-length request body");
        copied += count;
    }
    remaining_ -= copied;
    return copied;
}

::jxx::lang::jlong FixedLengthRequestBodySource::skip(::jxx::lang::jlong count)
{
    if (closed_ || count <= 0 || remaining_ == 0) return 0;
    const auto target = std::min(count, remaining_);
    ::jxx::lang::jlong skipped = 0;
    const auto prefixRemaining = static_cast<::jxx::lang::jlong>(prefixLimit_ - prefixPosition_);
    const auto prefixSkip = std::min(target, prefixRemaining);
    prefixPosition_ += static_cast<::jxx::lang::jint>(prefixSkip);
    skipped += prefixSkip;
    if (skipped < target) skipped += input_->skip(target - skipped);
    remaining_ -= skipped;
    return skipped;
}

::jxx::lang::jint FixedLengthRequestBodySource::available()
{
    if (closed_ || remaining_ == 0) return 0;
    const auto prefixAvailable = static_cast<::jxx::lang::jlong>(prefixLimit_ - prefixPosition_);
    const auto sourceAvailable = static_cast<::jxx::lang::jlong>(std::max(0, input_->available()));
    const auto total = std::min(remaining_, prefixAvailable + sourceAvailable);
    return static_cast<::jxx::lang::jint>(std::min<::jxx::lang::jlong>(
        total, std::numeric_limits<::jxx::lang::jint>::max()));
}

void FixedLengthRequestBodySource::close()
{
    closed_ = true;
}

::jxx::lang::jbool FixedLengthRequestBodySource::isFullyConsumedInternal() const noexcept
{
    return remaining_ == 0;
}

::jxx::lang::jbool FixedLengthRequestBodySource::wasClosedInternal() const noexcept
{
    return closed_;
}

} // namespace jxx::com::sun::net::httpserver::internal
