#include "net/jxx.net.StandardProtocolFamily.h"

namespace jxx::net
{
    jxx::Ptr<StandardProtocolFamily> StandardProtocolFamily::INET = std::make_shared<StandardProtocolFamily>(std::make_shared<jxx::lang::String>("INET"));
    jxx::Ptr<StandardProtocolFamily> StandardProtocolFamily::INET6 = std::make_shared<StandardProtocolFamily>(std::make_shared<jxx::lang::String>("INET6"));

    StandardProtocolFamily::StandardProtocolFamily(jxx::Ptr<jxx::lang::String> name)
        : name_(std::move(name))
    {
    }

    jxx::Ptr<jxx::lang::String> StandardProtocolFamily::toString() const { return name_; }
    jxx::lang::jbool StandardProtocolFamily::equals(jxx::Ptr<jxx::lang::Object> other) const
    {
        auto o = std::dynamic_pointer_cast<StandardProtocolFamily>(other);
        return o && name_ && o->name_ && name_->equals(o->name_);
    }
    jxx::lang::jint StandardProtocolFamily::hashCode() const { return name_ ? name_->hashCode() : 0; }
}
