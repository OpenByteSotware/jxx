#include "awt/event/jxx.awt.event.ClassInitialization.h"
#include <mutex>
#include "awt/jxx.awt.Container.h"
#include "io/jxx.io.ArrayCodecRegistry.h"
#include "io/jxx.io.SerializationClassRegistry.h"
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
        ::jxx::io::serialization::registerSerializableClass<::jxx::awt::event::ComponentEvent>(static_cast<::jxx::lang::jlong>(8101406823902992965LL), [] { return ::jxx::NEW<::jxx::awt::event::ComponentEvent>(::jxx::NEW<::jxx::awt::Container>(), 0); });
        ::jxx::io::serialization::registerClassReferenceArrayCodecs<::jxx::awt::event::ComponentEvent, 1U, 2U>();
        (void)::jxx::awt::event::ComponentListener::Class();
        (void)::jxx::awt::event::ContainerEvent::Class();
        ::jxx::io::serialization::registerSerializableClass<::jxx::awt::event::ContainerEvent>(static_cast<::jxx::lang::jlong>(-4114942250539772041LL), [] { return ::jxx::NEW<::jxx::awt::event::ContainerEvent>(::jxx::NEW<::jxx::awt::Container>(), 0, nullptr); });
        ::jxx::io::serialization::registerClassReferenceArrayCodecs<::jxx::awt::event::ContainerEvent, 1U, 2U>();
        (void)::jxx::awt::event::ContainerListener::Class();
        (void)::jxx::awt::event::FocusEvent::Class();
        ::jxx::io::serialization::registerSerializableClass<::jxx::awt::event::FocusEvent>(static_cast<::jxx::lang::jlong>(523753786457416396LL), [] { return ::jxx::NEW<::jxx::awt::event::FocusEvent>(::jxx::NEW<::jxx::awt::Container>(), 0, false, nullptr); });
        ::jxx::io::serialization::registerClassReferenceArrayCodecs<::jxx::awt::event::FocusEvent, 1U, 2U>();
        (void)::jxx::awt::event::FocusListener::Class();
        (void)::jxx::awt::event::HierarchyBoundsListener::Class();
        (void)::jxx::awt::event::HierarchyEvent::Class();
        ::jxx::io::serialization::registerSerializableClass<::jxx::awt::event::HierarchyEvent>(static_cast<::jxx::lang::jlong>(-5337576970038043990LL), [] { return ::jxx::NEW<::jxx::awt::event::HierarchyEvent>(::jxx::NEW<::jxx::awt::Container>(), 0, nullptr, nullptr, 0); });
        ::jxx::io::serialization::registerClassReferenceArrayCodecs<::jxx::awt::event::HierarchyEvent, 1U, 2U>();
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
