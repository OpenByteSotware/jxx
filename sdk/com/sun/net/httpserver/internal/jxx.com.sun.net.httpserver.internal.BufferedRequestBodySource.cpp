#include <algorithm>
#include "com/sun/net/httpserver/internal/jxx.com.sun.net.httpserver.internal.BufferedRequestBodySource.h"

#include "lang/jxx.lang.Exceptions.h"


namespace jxx::com::sun::net::httpserver::internal {

BufferedRequestBodySource::BufferedRequestBodySource(
    const ::jxx::lang::ByteArray& body,
    ::jxx::lang::jint offset,
    ::jxx::lang::jint length)
    : Super(), body_(body), position_(offset), limit_(offset + length)
{
    if (body_ == nullptr) throw ::jxx::lang::NullPointerException();
    if (offset < 0 || length < 0 || offset > body_->length - length)
        throw ::jxx::lang::IndexOutOfBoundsException();
}

::jxx::lang::jint BufferedRequestBodySource::read(
    const ::jxx::lang::ByteArray& buffer,
    ::jxx::lang::jint offset,
    ::jxx::lang::jint length)
{
    if (buffer == nullptr) throw ::jxx::lang::NullPointerException();
    if (offset < 0 || length < 0 || offset > buffer->length - length)
        throw ::jxx::lang::IndexOutOfBoundsException();
    if (length == 0) return 0;
    if (closed_ || position_ >= limit_) return -1;
    const auto count = std::min(length, limit_ - position_);
    for (::jxx::lang::jint index = 0; index < count; ++index)
        (*buffer)[offset + index] = (*body_)[position_ + index];
    position_ += count;
    return count;
}

::jxx::lang::jlong BufferedRequestBodySource::skip(::jxx::lang::jlong count)
{
    if (closed_ || count <= 0) return 0;
    const auto remaining = static_cast<::jxx::lang::jlong>(limit_ - position_);
    const auto skipped = std::min(count, remaining);
    position_ += static_cast<::jxx::lang::jint>(skipped);
    return skipped;
}

::jxx::lang::jint BufferedRequestBodySource::available()
{
    return closed_ ? 0 : limit_ - position_;
}

void BufferedRequestBodySource::close()
{
    closed_ = true;
}

::jxx::lang::jbool BufferedRequestBodySource::isFullyConsumedInternal() const noexcept
{
    return position_ >= limit_;
}

::jxx::lang::jbool BufferedRequestBodySource::wasClosedInternal() const noexcept
{
    return closed_;
}

} // namespace jxx::com::sun::net::httpserver::internal
