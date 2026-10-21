#include <gtest/gtest.h>

#include "awt/jxx.awt.ClassInitialization.h"
#include "awt/jxx.awt.Composite.h"
#include "awt/jxx.awt.CompositeContext.h"
#include "awt/jxx.awt.LayoutManager.h"
#include "awt/jxx.awt.Paint.h"
#include "awt/jxx.awt.PaintContext.h"
#include "awt/jxx.awt.Shape.h"
#include "awt/jxx.awt.Stroke.h"
#include "awt/jxx.awt.Transparency.h"
#include "awt/event/jxx.awt.event.ComponentEvent.h"
#include "awt/geom/jxx.awt.geom.PathIterator.h"

TEST(AwtPackageInitialization, InitializesOwnAndDirectChildPackageTypes)
{
    EXPECT_NO_THROW(::jxx::awt::initialize());
    EXPECT_NE(nullptr, ::jxx::awt::Composite::Class());
    EXPECT_NE(nullptr, ::jxx::awt::CompositeContext::Class());
    EXPECT_NE(nullptr, ::jxx::awt::LayoutManager::Class());
    EXPECT_NE(nullptr, ::jxx::awt::Paint::Class());
    EXPECT_NE(nullptr, ::jxx::awt::PaintContext::Class());
    EXPECT_NE(nullptr, ::jxx::awt::Shape::Class());
    EXPECT_NE(nullptr, ::jxx::awt::Stroke::Class());
    EXPECT_NE(nullptr, ::jxx::awt::Transparency::Class());
    EXPECT_NE(nullptr, ::jxx::awt::event::ComponentEvent::Class());
    EXPECT_NE(nullptr, ::jxx::awt::geom::PathIterator::Class());
}

TEST(AwtPackageInitialization, IsIdempotent)
{
    EXPECT_NO_THROW(::jxx::awt::initialize());
    EXPECT_NO_THROW(::jxx::awt::initialize());
}
