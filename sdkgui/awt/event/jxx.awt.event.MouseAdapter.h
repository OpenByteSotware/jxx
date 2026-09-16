#pragma once
#include "awt/event/jxx.awt.event.MouseListener.h"
#include "awt/event/jxx.awt.event.MouseMotionListener.h"
#include "awt/event/jxx.awt.event.MouseWheelListener.h"
#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Object.h"
namespace jxx::awt::event { class MouseAdapter : public ::jxx::lang::ClassBase<MouseAdapter,::jxx::lang::Object,MouseListener,MouseMotionListener,MouseWheelListener>{public:using JxxSuper=::jxx::lang::Object;using Super=::jxx::lang::ClassBase<MouseAdapter,JxxSuper,MouseListener,MouseMotionListener,MouseWheelListener>;~MouseAdapter() override=default;void mouseClicked(const ::jxx::Ptr<MouseEvent>&) override;void mousePressed(const ::jxx::Ptr<MouseEvent>&) override;void mouseReleased(const ::jxx::Ptr<MouseEvent>&) override;void mouseEntered(const ::jxx::Ptr<MouseEvent>&) override;void mouseExited(const ::jxx::Ptr<MouseEvent>&) override;void mouseDragged(const ::jxx::Ptr<MouseEvent>&) override;void mouseMoved(const ::jxx::Ptr<MouseEvent>&) override;void mouseWheelMoved(const ::jxx::Ptr<MouseWheelEvent>&) override;};}
