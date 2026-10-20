#include <gtest/gtest.h>

#include "awt/jxx.awt.ClassInitialization.h"
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
#include "awt/jxx.awt.ImageCapabilities.h"
#include "awt/jxx.awt.Insets.h"
#include "awt/jxx.awt.Point.h"
#include "awt/jxx.awt.Rectangle.h"
#include "awt/jxx.awt.RenderingHints.h"

namespace
{
    template <typename T>
    void expectClassInitialized()
    {
        const auto type = T::Class();
        ASSERT_NE(type, nullptr);
        ASSERT_NE(type->getName(), nullptr);
        EXPECT_FALSE(type->getName()->utf8().empty());
    }

    TEST(AwtClassInitialization, RegistersAllPublicClassBaseTypes)
    {
        ::jxx::awt::initializeClasses();
        expectClassInitialized<::jxx::awt::AWTEvent>();
        expectClassInitialized<::jxx::awt::AlphaComposite>();
        expectClassInitialized<::jxx::awt::BasicStroke>();
        expectClassInitialized<::jxx::awt::BufferCapabilities>();
        expectClassInitialized<::jxx::awt::BufferCapabilities::FlipContents>();
        expectClassInitialized<::jxx::awt::Color>();
        expectClassInitialized<::jxx::awt::ComponentOrientation>();
        expectClassInitialized<::jxx::awt::Dimension>();
        expectClassInitialized<::jxx::awt::Font>();
        expectClassInitialized<::jxx::awt::GradientPaint>();
        expectClassInitialized<::jxx::awt::Graphics>();
        expectClassInitialized<::jxx::awt::Graphics2D>();
        expectClassInitialized<::jxx::awt::Image>();
        expectClassInitialized<::jxx::awt::ImageCapabilities>();
        expectClassInitialized<::jxx::awt::Insets>();
        expectClassInitialized<::jxx::awt::Point>();
        expectClassInitialized<::jxx::awt::Rectangle>();
        expectClassInitialized<::jxx::awt::RenderingHints>();
        expectClassInitialized<::jxx::awt::RenderingHints::Key>();
    }
}
