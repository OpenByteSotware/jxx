#include "com/sun/net/httpserver/internal/jxx.com.sun.net.httpserver.internal.RequestBodyInputStream.h"

#include "lang/jxx.lang.Exceptions.h"

#include <algorithm>

namespace jxx::com::sun::net::httpserver::internal {

RequestBodyInputStream::RequestBodyInputStream(const ::jxx::lang::ByteArray& body)
    : Super(), body_(body)
{
    if (body_ == nullptr) throw ::jxx::lang::NullPointerException();
}

::jxx::lang::jint RequestBodyInputStream::read()
{
    if (closed_ || position_ >= body_->length) return -1;
    return static_cast<unsigned char>((*body_)[position_++]);
}

::jxx::lang::jint RequestBodyInputStream::read(
    const ::jxx::lang::ByteArray& buffer,
    ::jxx::lang::jint offset,
    ::jxx::lang::jint length)
{
    if (buffer == nullptr) throw ::jxx::lang::NullPointerException();
    if (offset < 0 || length < 0 || offset > buffer->length - length)
        throw ::jxx::lang::IndexOutOfBoundsException();
    if (length == 0) return 0;
    if (closed_ || position_ >= body_->length) return -1;
    const auto count = std::min(length, body_->length - position_);
    for (::jxx::lang::jint index = 0; index < count; ++index)
        (*buffer)[offset + index] = (*body_)[position_ + index];
    position_ += count;
    return count;
}

::jxx::lang::jlong RequestBodyInputStream::skip(::jxx::lang::jlong count)
{
    if (closed_ || count <= 0) return 0;
    const auto remaining = static_cast<::jxx::lang::jlong>(body_->length - position_);
    const auto skipped = std::min(count, remaining);
    position_ += static_cast<::jxx::lang::jint>(skipped);
    return skipped;
}

::jxx::lang::jint RequestBodyInputStream::available()
{
    return closed_ ? 0 : body_->length - position_;
}

void RequestBodyInputStream::close()
{
    closed_ = true;
}

::jxx::lang::jbool RequestBodyInputStream::markSupported() const
{
    return false;
}

::jxx::lang::jbool RequestBodyInputStream::isFullyConsumedInternal() const noexcept
{
    return position_ >= body_->length;
}

::jxx::lang::jbool RequestBodyInputStream::wasClosedInternal() const noexcept
{
    return closed_;
}

} // namespace jxx::com::sun::net::httpserver::internal
