#include "awt/event/jxx.awt.event.TextEvent.h"
namespace jxx::awt::event {TextEvent::TextEvent(const ::jxx::Ptr<::jxx::lang::Object>& source,::jxx::lang::jint id):Super(source,id){}::jxx::Ptr<::jxx::lang::String> TextEvent::paramString() const{return ::jxx::NEW<::jxx::lang::String>(id==TEXT_VALUE_CHANGED?"TEXT_VALUE_CHANGED":"unknown type");}}
