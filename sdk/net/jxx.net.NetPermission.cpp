#include "net/jxx.net.NetPermission.h"

namespace jxx::net
{
    NetPermission::NetPermission(const jxx::Ptr<jxx::lang::String>& name)
        : NetPermission(name, nullptr)
    {
    }

    NetPermission::NetPermission(const jxx::Ptr<jxx::lang::String>& name,
                                 const jxx::Ptr<jxx::lang::String>& actions)
        : name_(name),
          actions_(actions)
    {
    }

    jxx::Ptr<jxx::lang::String> NetPermission::getName() const { return name_; }
    jxx::Ptr<jxx::lang::String> NetPermission::getActions() const { return actions_; }

    jxx::lang::jbool NetPermission::implies(const jxx::Ptr<NetPermission>& p) const
    {
        return p && name_ && p->name_ && name_->equals(p->name_);
    }

    jxx::Ptr<jxx::lang::String> NetPermission::toString() const { return name_; }

    jxx::lang::jbool NetPermission::equals(const jxx::Ptr<jxx::lang::Object>& other) const
    {
        auto p = std::dynamic_pointer_cast<NetPermission>(other);
        return p &&
            ((!name_ && !p->name_) || (name_ && p->name_ && name_->equals(p->name_))) &&
            ((!actions_ && !p->actions_) || (actions_ && p->actions_ && actions_->equals(p->actions_)));
    }

    jxx::lang::jint NetPermission::hashCode() const
    {
        return (name_ ? name_->hashCode() : 0) + (actions_ ? actions_->hashCode() : 0);
    }
}
