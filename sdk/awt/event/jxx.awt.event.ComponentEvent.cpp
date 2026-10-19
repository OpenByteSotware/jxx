#include "awt/event/jxx.awt.event.ComponentEvent.h"
namespace jxx::awt::event {
ComponentEvent::ComponentEvent(const ::jxx::Ptr<::jxx::awt::Component>& source,::jxx::lang::jint id):Super(source,id){}
::jxx::Ptr<::jxx::awt::Component> ComponentEvent::getComponent() const{return ::jxx::CAST<::jxx::awt::Component>(source);}
::jxx::Ptr<::jxx::lang::String> ComponentEvent::paramString() const{const char* p="unknown type";if(id==COMPONENT_MOVED)p="COMPONENT_MOVED";else if(id==COMPONENT_RESIZED)p="COMPONENT_RESIZED";else if(id==COMPONENT_SHOWN)p="COMPONENT_SHOWN";else if(id==COMPONENT_HIDDEN)p="COMPONENT_HIDDEN";return ::jxx::NEW<::jxx::lang::String>(p);}
}
