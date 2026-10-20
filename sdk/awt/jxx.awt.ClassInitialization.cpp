#include "awt/jxx.awt.ClassInitialization.h"
#include "awt/event/jxx.awt.event.ComponentEvent.h"
#include "awt/event/jxx.awt.event.ContainerEvent.h"
#include "awt/event/jxx.awt.event.FocusEvent.h"
#include "awt/event/jxx.awt.event.HierarchyEvent.h"
#include "awt/event/jxx.awt.event.InputEvent.h"
#include "awt/event/jxx.awt.event.KeyEvent.h"
#include "awt/event/jxx.awt.event.MouseEvent.h"
#include "awt/event/jxx.awt.event.MouseWheelEvent.h"
#include "awt/jxx.awt.AWTEvent.h"
#include "awt/jxx.awt.AlphaComposite.h"
#include "awt/jxx.awt.BasicStroke.h"
#include "awt/jxx.awt.BufferCapabilities.h"
#include "awt/jxx.awt.Color.h"
#include "awt/jxx.awt.ComponentOrientation.h"
#include "awt/jxx.awt.Dimension.h"
#include "awt/jxx.awt.Font.h"
#include "awt/jxx.awt.GradientPaint.h"
#include "awt/jxx.awt.Graphics.h"
#include "awt/jxx.awt.Graphics2D.h"
#include "awt/jxx.awt.Image.h"
#include "awt/jxx.awt.MenuContainer.h"
#include "awt/jxx.awt.ImageCapabilities.h"
#include "awt/jxx.awt.Insets.h"
#include "awt/jxx.awt.Point.h"
#include "awt/jxx.awt.Rectangle.h"
#include "awt/jxx.awt.RenderingHints.h"

#include "awt/jxx.awt.Component.h"
#include "awt/jxx.awt.Container.h"
#include "awt/jxx.awt.Polygon.h"
#include "awt/jxx.awt.Toolkit.h"
#include "io/jxx.io.ArrayCodecRegistry.h"
#include "io/jxx.io.SerializationClassRegistry.h"

#include <mutex>

namespace jxx::awt
{
    void initializeClasses()
    {
        static std::once_flag flag;
        std::call_once(flag, []()
        {
            (void)::jxx::awt::AWTEvent::Class();
            (void)::jxx::awt::event::ComponentEvent::Class();
            (void)::jxx::awt::event::ContainerEvent::Class();
            (void)::jxx::awt::event::FocusEvent::Class();
            (void)::jxx::awt::event::HierarchyEvent::Class();
            (void)::jxx::awt::event::InputEvent::Class();
            (void)::jxx::awt::event::KeyEvent::Class();
            (void)::jxx::awt::event::MouseEvent::Class();
            (void)::jxx::awt::event::MouseWheelEvent::Class();
            (void)::jxx::awt::AlphaComposite::Class();
            (void)::jxx::awt::BasicStroke::Class();
            (void)::jxx::awt::BufferCapabilities::Class();
            (void)::jxx::awt::BufferCapabilities::FlipContents::Class();
            (void)::jxx::awt::Color::Class();
            (void)::jxx::awt::ComponentOrientation::Class();
            (void)::jxx::awt::Dimension::Class();
            (void)::jxx::awt::Font::Class();
            (void)::jxx::awt::GradientPaint::Class();
            (void)::jxx::awt::Graphics::Class();
            (void)::jxx::awt::Graphics2D::Class();
            (void)::jxx::awt::Image::Class();
            (void)::jxx::awt::MenuContainer::Class();
            (void)::jxx::awt::ImageCapabilities::Class();
            (void)::jxx::awt::Insets::Class();
            (void)::jxx::awt::Point::Class();
            (void)::jxx::awt::Rectangle::Class();
            (void)::jxx::awt::RenderingHints::Class();
            (void)::jxx::awt::RenderingHints::Key::Class();
            (void)::jxx::awt::Component::Class();
            (void)::jxx::awt::Container::Class();
            (void)::jxx::awt::Polygon::Class();
            ::jxx::io::serialization::registerSerializableClass<
                ::jxx::awt::Polygon>(
                    static_cast<::jxx::lang::jlong>(-6460061437900069969LL));
            ::jxx::io::serialization::registerClassReferenceArrayCodecs<
                ::jxx::awt::Polygon, 1U, 2U>();
            (void)::jxx::awt::Toolkit::Class();
        });
    }
}
