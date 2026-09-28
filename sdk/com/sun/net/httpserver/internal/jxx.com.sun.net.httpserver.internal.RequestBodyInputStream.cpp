#include "com/sun/net/httpserver/internal/jxx.com.sun.net.httpserver.internal.RequestBodyInputStream.h"

#include "com/sun/net/httpserver/internal/jxx.com.sun.net.httpserver.internal.BufferedRequestBodySource.h"
#include "com/sun/net/httpserver/internal/jxx.com.sun.net.httpserver.internal.RequestBodySource.h"
#include "lang/jxx.lang.Exceptions.h"

namespace jxx::com::sun::net::httpserver::internal {

RequestBodyInputStream::RequestBodyInputStream(const ::jxx::lang::ByteArray& body)
    : RequestBodyInputStream(
          ::jxx::CAST<RequestBodySource>(
              ::jxx::NEW<BufferedRequestBodySource>(body, 0, body == nullptr ? 0 : body->length)))
{
}

RequestBodyInputStream::RequestBodyInputStream(const ::jxx::Ptr<RequestBodySource>& source)
    : Super(), source_(source)
{
    if (source_ == nullptr) throw ::jxx::lang::NullPointerException();
}

::jxx::lang::jint RequestBodyInputStream::read()
{
    auto one = ::jxx::NEW<::jxx::lang::ByteArrayType>(1);
    const auto count = source_->read(one, 0, 1);
    return count < 0 ? -1 : static_cast<unsigned char>((*one)[0]);
}

::jxx::lang::jint RequestBodyInputStream::read(
    const ::jxx::lang::ByteArray& buffer,
    ::jxx::lang::jint offset,
    ::jxx::lang::jint length)
{
    return source_->read(buffer, offset, length);
}

::jxx::lang::jlong RequestBodyInputStream::skip(::jxx::lang::jlong count)
{
    return source_->skip(count);
}

::jxx::lang::jint RequestBodyInputStream::available()
{
    return source_->available();
}

void RequestBodyInputStream::close()
{
    source_->close();
}

::jxx::lang::jbool RequestBodyInputStream::markSupported() const
{
    return false;
}

::jxx::lang::jbool RequestBodyInputStream::isFullyConsumedInternal() const noexcept
{
    return source_->isFullyConsumedInternal();
}

::jxx::lang::jbool RequestBodyInputStream::wasClosedInternal() const noexcept
{
    return source_->wasClosedInternal();
}

} // namespace jxx::com::sun::net::httpserver::internal
