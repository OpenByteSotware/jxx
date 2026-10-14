#include "net/jxx.net.HttpRetryException.h"

namespace jxx::net
{
    HttpRetryException::HttpRetryException(const jxx::Ptr<jxx::lang::String>& detail,
                                           jxx::lang::jint code)
        : HttpRetryException(detail, code, nullptr)
    {
    }

    HttpRetryException::HttpRetryException(const jxx::Ptr<jxx::lang::String>& detail,
                                           jxx::lang::jint code,
                                           const jxx::Ptr<jxx::lang::String>& location)
        : Super(detail ? detail->utf8() : std::string("HttpRetryException")),
          detail_(detail),
          code_(code),
          location_(location)
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
