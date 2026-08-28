#include "net/jxx.net.SocketPermission.h"

namespace jxx::net
{
    SocketPermission::SocketPermission(const jxx::Ptr<jxx::lang::String> host,
                                       jxx::Ptr<jxx::lang::String> action)
        : host_(std::move(host)),
          action_(std::move(action))
    {
    }

    jxx::Ptr<jxx::lang::String> SocketPermission::getName() const { return host_; }
    jxx::Ptr<jxx::lang::String> SocketPermission::getActions() const { return action_; }

    jxx::lang::jbool SocketPermission::implies(const jxx::Ptr<SocketPermission> p) const
    {
        return p &&
            ((!host_ && !p->host_) || (host_ && p->host_ && host_->equals(p->host_))) &&
            ((!action_ && !p->action_) || (action_ && p->action_ && action_->equals(p->action_)));
    }

    jxx::Ptr<jxx::lang::String> SocketPermission::toString() const { return host_; }

    jxx::lang::jbool SocketPermission::equals(const jxx::Ptr<jxx::lang::Object>& other) const
    {
        auto p = std::dynamic_pointer_cast<SocketPermission>(other);
        return implies(p);
    }

    jxx::lang::jint SocketPermission::hashCode() const
    {
        return (host_ ? host_->hashCode() : 0) + (action_ ? action_->hashCode() : 0);
    }
}
