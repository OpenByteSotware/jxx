#include <cstdio>
#include <cstring>
#include "com/sun/net/httpserver/internal/jxx.com.sun.net.httpserver.internal.ResponseBodyOutputStream.h"
#include "io/jxx.io.IOException.h"
#include "lang/jxx.lang.NullPointerException.h"

namespace jxx::com::sun::net::httpserver::internal {

ResponseBodyOutputStream::ResponseBodyOutputStream(
    const ::jxx::Ptr<::jxx::io::OutputStream>& delegate,
    ResponseBodyMode mode,
    ::jxx::lang::jlong expectedLength)
    : Super(), delegate_(delegate), mode_(mode), expectedLength_(expectedLength)
{
    if (delegate_ == nullptr) {
        throw ::jxx::lang::NullPointerException();
    }
}

void ResponseBodyOutputStream::writeAscii_(const char* text)
{
    const auto length = static_cast<::jxx::lang::jint>(std::strlen(text));
    auto bytes = ::jxx::NEW<::jxx::lang::ByteArrayType>(length);
    for (::jxx::lang::jint index = 0; index < length; ++index) {
        (*bytes)[index] = static_cast<::jxx::lang::jbyte>(text[index]);
    }
    delegate_->write(bytes, 0, length);
}

void ResponseBodyOutputStream::write(::jxx::lang::jint value)
{
    auto one = ::jxx::NEW<::jxx::lang::ByteArrayType>(1);
    (*one)[0] = static_cast<::jxx::lang::jbyte>(value);
    write(one, 0, 1);
}

void ResponseBodyOutputStream::write(
    const ::jxx::lang::ByteArray& buffer,
    ::jxx::lang::jint offset,
    ::jxx::lang::jint length)
{
    if (closed_) throw ::jxx::io::IOException("response body is closed");
    if (length == 0) return;
    if (mode_ == ResponseBodyMode::NoBody)
        throw ::jxx::io::IOException("response does not permit a body");
    if (mode_ == ResponseBodyMode::FixedLength && written_ + length > expectedLength_)
        throw ::jxx::io::IOException("fixed response length exceeded");

    if (mode_ == ResponseBodyMode::Chunked) {
        char header[32]{};
        std::snprintf(header, sizeof(header), "%x\r\n", static_cast<unsigned int>(length));
        writeAscii_(header);
        delegate_->write(buffer, offset, length);
        writeAscii_("\r\n");
    }
    else {
        delegate_->write(buffer, offset, length);
    }
    written_ += length;
}

void ResponseBodyOutputStream::flush()
{
    delegate_->flush();
}

void ResponseBodyOutputStream::close()
{
    if (closed_) return;
    if (mode_ == ResponseBodyMode::FixedLength && written_ != expectedLength_)
        throw ::jxx::io::IOException("fixed response length not satisfied");
    if (mode_ == ResponseBodyMode::Chunked)
        writeAscii_("0\r\n\r\n");
    delegate_->flush();
    closed_ = true;
}

} // namespace jxx::com::sun::net::httpserver::internal
