#pragma once
#include "awt/jxx.awt.AWTEvent.h"
namespace jxx::awt { class Container; }
namespace jxx::swing { class JComponent; }
namespace jxx::swing::event {
class AncestorEvent : public ::jxx::lang::ClassBase<AncestorEvent, ::jxx::awt::AWTEvent> {
public:
    using JxxSuper = ::jxx::awt::AWTEvent;
    using Super = ::jxx::lang::ClassBase<AncestorEvent, JxxSuper>;
    static constexpr ::jxx::lang::jint ANCESTOR_ADDED = 1;
    static constexpr ::jxx::lang::jint ANCESTOR_REMOVED = 2;
    static constexpr ::jxx::lang::jint ANCESTOR_MOVED = 3;
    AncestorEvent(const ::jxx::Ptr<::jxx::swing::JComponent>& source, ::jxx::lang::jint id,
        const ::jxx::Ptr<::jxx::awt::Container>& ancestor,
        const ::jxx::Ptr<::jxx::awt::Container>& ancestorParent);
    ::jxx::Ptr<::jxx::awt::Container> getAncestor() const;
    ::jxx::Ptr<::jxx::awt::Container> getAncestorParent() const;
    ::jxx::Ptr<::jxx::swing::JComponent> getComponent() const;
private:
    ::jxx::Ptr<::jxx::awt::Container> ancestor_;
    ::jxx::Ptr<::jxx::awt::Container> ancestorParent_;
};
}
