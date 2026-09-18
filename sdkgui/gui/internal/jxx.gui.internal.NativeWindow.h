#pragma once
#include <functional>
#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.String.h"
#include "lang/jxx_types.h"
namespace jxx::awt { class MenuBar; }
namespace jxx::swing { class JMenuBar; }
namespace jxx::gui::internal {
class NativeWindow : public ::jxx::lang::Object {
public:
 using EventCallback=std::function<void(::jxx::lang::jint)>;
 ~NativeWindow() override=default;
 virtual void show()=0; virtual void hide()=0; virtual void destroy()=0; virtual void toFront()=0; virtual void toBack()=0;
 virtual void setBounds(::jxx::lang::jint x,::jxx::lang::jint y,::jxx::lang::jint width,::jxx::lang::jint height)=0;
 virtual void setTitle(const ::jxx::Ptr<::jxx::lang::String>& title)=0;
 virtual ::jxx::Ptr<::jxx::lang::String> getTitle() const=0; virtual ::jxx::lang::jbool isShown() const=0;
 virtual void setEventCallback(EventCallback callback)=0;
 virtual void setMenuBar(const ::jxx::Ptr<::jxx::awt::MenuBar>& menuBar)=0;
 virtual void setSwingMenuBar(const ::jxx::Ptr<::jxx::swing::JMenuBar>& menuBar)=0;
}; }
