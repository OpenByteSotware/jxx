#pragma once
#include <vector>
#include "awt/jxx.awt.Container.h"
namespace jxx::awt::event { class WindowEvent; class WindowListener; }
namespace jxx::gui::internal { class NativeWindow; }
namespace jxx::awt {
class Window : public ::jxx::lang::ClassBase<Window,Container> {
public:
 using JxxSuper=Container; using Super=::jxx::lang::ClassBase<Window,JxxSuper>;
 Window(); ~Window() override;
 void setVisible(::jxx::lang::jbool visible) override;
 void setBounds(const ::jxx::Ptr<Rectangle>& rectangle) override;
 void setBounds(::jxx::lang::jint x,::jxx::lang::jint y,::jxx::lang::jint width,::jxx::lang::jint height) override;
 virtual void pack(); virtual void dispose(); virtual void toFront(); virtual void toBack();
 virtual ::jxx::lang::jbool isActive() const; virtual ::jxx::lang::jbool isDisplayable() const;
 virtual void addWindowListener(const ::jxx::Ptr<::jxx::awt::event::WindowListener>& listener);
 virtual void removeWindowListener(const ::jxx::Ptr<::jxx::awt::event::WindowListener>& listener);
protected:
 virtual void ensureNativeWindow(); void configureNativeEvents(); virtual void processWindowEvent(const ::jxx::Ptr<::jxx::awt::event::WindowEvent>& event);
 ::jxx::Ptr<::jxx::gui::internal::NativeWindow> nativeWindow_; ::jxx::lang::jbool active_; ::jxx::lang::jbool displayable_;
private: std::vector<::jxx::Ptr<::jxx::awt::event::WindowListener>> windowListeners_;
}; }
