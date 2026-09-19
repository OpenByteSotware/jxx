#include "swing/event/jxx.swing.event.AncestorEvent.h"
#include "swing/jxx.swing.JComponent.h"
#include "awt/jxx.awt.Container.h"
namespace jxx::swing::event {
AncestorEvent::AncestorEvent(const ::jxx::Ptr<::jxx::swing::JComponent>& source, ::jxx::lang::jint id,
    const ::jxx::Ptr<::jxx::awt::Container>& ancestor,
    const ::jxx::Ptr<::jxx::awt::Container>& ancestorParent)
    : Super(source, id), ancestor_(ancestor), ancestorParent_(ancestorParent) {}
::jxx::Ptr<::jxx::awt::Container> AncestorEvent::getAncestor() const { return ancestor_; }
::jxx::Ptr<::jxx::awt::Container> AncestorEvent::getAncestorParent() const { return ancestorParent_; }
::jxx::Ptr<::jxx::swing::JComponent> AncestorEvent::getComponent() const { return ::jxx::CAST<::jxx::swing::JComponent>(getSource()); }
}
