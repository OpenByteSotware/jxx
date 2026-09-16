#pragma once
#include "awt/event/jxx.awt.event.WindowFocusListener.h"
#include "awt/event/jxx.awt.event.WindowListener.h"
#include "awt/event/jxx.awt.event.WindowStateListener.h"
#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Object.h"
namespace jxx::awt::event { class WindowAdapter : public ::jxx::lang::ClassBase<WindowAdapter,::jxx::lang::Object,WindowListener,WindowFocusListener,WindowStateListener>{public:using JxxSuper=::jxx::lang::Object;using Super=::jxx::lang::ClassBase<WindowAdapter,JxxSuper,WindowListener,WindowFocusListener,WindowStateListener>;~WindowAdapter() override=default;void windowOpened(const ::jxx::Ptr<WindowEvent>&) override;void windowClosing(const ::jxx::Ptr<WindowEvent>&) override;void windowClosed(const ::jxx::Ptr<WindowEvent>&) override;void windowIconified(const ::jxx::Ptr<WindowEvent>&) override;void windowDeiconified(const ::jxx::Ptr<WindowEvent>&) override;void windowActivated(const ::jxx::Ptr<WindowEvent>&) override;void windowDeactivated(const ::jxx::Ptr<WindowEvent>&) override;void windowGainedFocus(const ::jxx::Ptr<WindowEvent>&) override;void windowLostFocus(const ::jxx::Ptr<WindowEvent>&) override;void windowStateChanged(const ::jxx::Ptr<WindowEvent>&) override;}; }
