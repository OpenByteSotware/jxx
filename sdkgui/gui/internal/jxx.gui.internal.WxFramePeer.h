#pragma once
#include "gui/internal/jxx.gui.internal.NativeWindow.h"
class wxFrame;
namespace jxx::gui::internal {
class WxFramePeer final : public NativeWindow {
public:
 explicit WxFramePeer(const ::jxx::Ptr<::jxx::lang::String>& title); ~WxFramePeer() override;
 void show() override; void hide() override; void destroy() override; void toFront() override; void toBack() override;
 void setBounds(::jxx::lang::jint x,::jxx::lang::jint y,::jxx::lang::jint width,::jxx::lang::jint height) override;
 void setTitle(const ::jxx::Ptr<::jxx::lang::String>& title) override;
 ::jxx::Ptr<::jxx::lang::String> getTitle() const override; ::jxx::lang::jbool isShown() const override;
 void setEventCallback(EventCallback callback) override;
private:
 void send(::jxx::lang::jint id);
 wxFrame* frame_; ::jxx::Ptr<::jxx::lang::String> title_; EventCallback callback_;
}; }
