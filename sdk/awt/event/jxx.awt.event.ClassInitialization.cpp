#include "awt/event/jxx.awt.event.ClassInitialization.h"
#include <mutex>
#include "awt/event/jxx.awt.event.ComponentEvent.h"
#include "awt/event/jxx.awt.event.ComponentListener.h"
#include "awt/event/jxx.awt.event.ContainerEvent.h"
#include "awt/event/jxx.awt.event.ContainerListener.h"
#include "awt/event/jxx.awt.event.FocusEvent.h"
#include "awt/event/jxx.awt.event.FocusListener.h"
#include "awt/event/jxx.awt.event.HierarchyBoundsListener.h"
#include "awt/event/jxx.awt.event.HierarchyEvent.h"
#include "awt/event/jxx.awt.event.HierarchyListener.h"
#include "awt/event/jxx.awt.event.InputEvent.h"
#include "awt/event/jxx.awt.event.KeyEvent.h"
#include "awt/event/jxx.awt.event.KeyListener.h"
#include "awt/event/jxx.awt.event.MouseEvent.h"
#include "awt/event/jxx.awt.event.MouseListener.h"
#include "awt/event/jxx.awt.event.MouseMotionListener.h"
#include "awt/event/jxx.awt.event.MouseWheelEvent.h"
#include "awt/event/jxx.awt.event.MouseWheelListener.h"

namespace jxx::awt::event
{
void initializeClasses()
{
    static std::once_flag flag;
    std::call_once(flag, []()
    {
        (void)::jxx::awt::event::ComponentEvent::Class();
        (void)::jxx::awt::event::ComponentListener::Class();
        (void)::jxx::awt::event::ContainerEvent::Class();
        (void)::jxx::awt::event::ContainerListener::Class();
        (void)::jxx::awt::event::FocusEvent::Class();
        (void)::jxx::awt::event::FocusListener::Class();
        (void)::jxx::awt::event::HierarchyBoundsListener::Class();
        (void)::jxx::awt::event::HierarchyEvent::Class();
        (void)::jxx::awt::event::HierarchyListener::Class();
        (void)::jxx::awt::event::InputEvent::Class();
        (void)::jxx::awt::event::KeyEvent::Class();
        (void)::jxx::awt::event::KeyListener::Class();
        (void)::jxx::awt::event::MouseEvent::Class();
        (void)::jxx::awt::event::MouseListener::Class();
        (void)::jxx::awt::event::MouseMotionListener::Class();
        (void)::jxx::awt::event::MouseWheelEvent::Class();
        (void)::jxx::awt::event::MouseWheelListener::Class();
    });
}

void initialize()
{
    initializeClasses();
}

} // namespace jxx::awt::event
