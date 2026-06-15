#include "net/jxx.net.HttpRetryException.h"

namespace jxx::net
{
    HttpRetryException::HttpRetryException(jxx::Ptr<jxx::lang::String> detail,
                                           jxx::lang::jint code)
        : HttpRetryException(std::move(detail), code, nullptr)
    {
    }

    HttpRetryException::HttpRetryException(jxx::Ptr<jxx::lang::String> detail,
                                           jxx::lang::jint code,
                                           jxx::Ptr<jxx::lang::String> location)
        : std::runtime_error(detail ? detail->utf8() : std::string("HttpRetryException")),
          detail_(std::move(detail)),
          code_(code),
          location_(std::move(location))
    {
    }

    jxx::lang::jint HttpRetryException::responseCode() const noexcept
    {
        return code_;
    }

    jxx::Ptr<jxx::lang::String> HttpRetryException::getReason() const
    {
        return detail_;
    }

    jxx::Ptr<jxx::lang::String> HttpRetryException::getLocation() const
    {
        return location_;
    }
}
