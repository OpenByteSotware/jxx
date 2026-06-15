#include "net/jxx.net.URLPermission.h"

namespace jxx::net
{
    URLPermission::URLPermission(jxx::Ptr<jxx::lang::String> url)
        : URLPermission(std::move(url), nullptr)
    {
    }

    URLPermission::URLPermission(jxx::Ptr<jxx::lang::String> url,
                                 jxx::Ptr<jxx::lang::String> actions)
        : url_(std::move(url)),
          actions_(std::move(actions))
    {
    }

    jxx::Ptr<jxx::lang::String> URLPermission::getName() const { return url_; }
    jxx::Ptr<jxx::lang::String> URLPermission::getActions() const { return actions_; }

    jxx::lang::jbool URLPermission::implies(jxx::Ptr<URLPermission> p) const
    {
        return p &&
            ((!url_ && !p->url_) || (url_ && p->url_ && url_->equals(p->url_))) &&
            ((!actions_ && !p->actions_) || (actions_ && p->actions_ && actions_->equals(p->actions_)));
    }

    jxx::Ptr<jxx::lang::String> URLPermission::toString() const { return url_; }
    jxx::lang::jbool URLPermission::equals(jxx::Ptr<jxx::lang::Object> other) const
    {
        return implies(std::dynamic_pointer_cast<URLPermission>(other));
    }
    jxx::lang::jint URLPermission::hashCode() const
    {
        return (url_ ? url_->hashCode() : 0) + (actions_ ? actions_->hashCode() : 0);
    }
}
