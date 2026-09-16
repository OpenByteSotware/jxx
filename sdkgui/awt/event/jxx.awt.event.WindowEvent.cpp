#include "awt/event/jxx.awt.event.WindowEvent.h"
namespace jxx::awt::event {
WindowEvent::WindowEvent(const ::jxx::Ptr<::jxx::awt::Window>& source,::jxx::lang::jint id):Super(source,id){}
::jxx::Ptr<::jxx::awt::Window> WindowEvent::getWindow() const{return ::jxx::CAST<::jxx::awt::Window>(source);}
::jxx::Ptr<::jxx::lang::String> WindowEvent::paramString() const{const char* p="unknown type";switch(id){case WINDOW_OPENED:p="WINDOW_OPENED";break;case WINDOW_CLOSING:p="WINDOW_CLOSING";break;case WINDOW_CLOSED:p="WINDOW_CLOSED";break;case WINDOW_ICONIFIED:p="WINDOW_ICONIFIED";break;case WINDOW_DEICONIFIED:p="WINDOW_DEICONIFIED";break;case WINDOW_ACTIVATED:p="WINDOW_ACTIVATED";break;case WINDOW_DEACTIVATED:p="WINDOW_DEACTIVATED";break;case WINDOW_GAINED_FOCUS:p="WINDOW_GAINED_FOCUS";break;case WINDOW_LOST_FOCUS:p="WINDOW_LOST_FOCUS";break;case WINDOW_STATE_CHANGED:p="WINDOW_STATE_CHANGED";break;}return ::jxx::NEW<::jxx::lang::String>(p);}
}
