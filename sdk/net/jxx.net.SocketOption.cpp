#include "net/jxx.net.SocketOption.h"

namespace jxx::net
{
    BasicSocketOption::BasicSocketOption(const jxx::Ptr<jxx::lang::String>& name)
        : name_(name)
    {
    }

    jxx::Ptr<jxx::lang::String> BasicSocketOption::name() const { return name_; }
    jxx::Ptr<jxx::lang::Class> BasicSocketOption::type() const { return nullptr; }
    jxx::Ptr<jxx::lang::String> BasicSocketOption::toString() const { return name_; }
}
