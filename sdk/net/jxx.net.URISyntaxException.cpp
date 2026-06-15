#include "net/jxx.net.URISyntaxException.h"

namespace jxx::net
{
    URISyntaxException::URISyntaxException(jxx::Ptr<jxx::lang::String> input,
                                           jxx::Ptr<jxx::lang::String> reason,
                                           jxx::lang::jint index)
        : std::runtime_error(reason ? reason->utf8() : std::string("URISyntaxException")),
          input_(std::move(input)),
          reason_(std::move(reason)),
          index_(index)
    {
    }

    URISyntaxException::URISyntaxException(jxx::Ptr<jxx::lang::String> input,
                                           jxx::Ptr<jxx::lang::String> reason)
        : URISyntaxException(std::move(input), std::move(reason), -1)
    {
    }

    jxx::Ptr<jxx::lang::String> URISyntaxException::getInput() const { return input_; }
    jxx::Ptr<jxx::lang::String> URISyntaxException::getReason() const { return reason_; }
    jxx::lang::jint URISyntaxException::getIndex() const noexcept { return index_; }

    jxx::Ptr<jxx::lang::String> URISyntaxException::getMessage() const
    {
        std::string msg = reason_ ? reason_->utf8() : std::string();
        if (index_ >= 0)
            msg += " at index " + std::to_string(index_);
        if (input_)
            msg += ": " + input_->utf8();
        return std::make_shared<jxx::lang::String>(msg);
    }
}
