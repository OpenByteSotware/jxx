#include <gtest/gtest.h>

#include "jxx.initialize.h"
#include "awt/jxx.awt.Color.h"
#include "awt/jxx.awt.Component.h"
#include "awt/jxx.awt.Graphics.h"
#include "awt/image/jxx.awt.image.BufferedImage.h"
#include "ext/swing/jxx.ext.swing.SwingUtilities.h"
#include "lang/jxx.lang.Runnable.h"

namespace {

class FilledComponent final
    : public ::jxx::lang::ClassBase<FilledComponent, ::jxx::awt::Component> {
public:
    using JxxSuper = ::jxx::awt::Component;
    using Super = ::jxx::lang::ClassBase<FilledComponent, JxxSuper>;

    void setFill(const ::jxx::Ptr<::jxx::awt::Color>& fill) { fill_ = fill; }
    void paint(const ::jxx::Ptr<::jxx::awt::Graphics>& graphics) override
    {
        graphics->setColor(fill_);
        graphics->fillRect(0, 0, getWidth(), getHeight());
    }
private:
    ::jxx::Ptr<::jxx::awt::Color> fill_ = ::jxx::awt::Color::black;
};

class UpdateTask final
    : public ::jxx::lang::ClassBase<
          UpdateTask, ::jxx::lang::Object, ::jxx::lang::Runnable> {
public:
    using JxxSuper = ::jxx::lang::Object;
    using Super = ::jxx::lang::ClassBase<
        UpdateTask, JxxSuper, ::jxx::lang::Runnable>;

    UpdateTask(const ::jxx::Ptr<FilledComponent>& component,
               const ::jxx::Ptr<::jxx::awt::Color>& fill)
        : Super(), component_(component), fill_(fill) {}
    void run() override
    {
        component_->setFill(fill_);
        component_->repaint();
        component_->repaint(1);
        component_->repaint(0, 0, 2, 2);
        component_->repaint(1, 0, 0, 2, 2);
    }
private:
    ::jxx::Ptr<FilledComponent> component_;
    ::jxx::Ptr<::jxx::awt::Color> fill_;
};

TEST(CacheCreatorSdkIntegration, InvokeAndWaitRepaintThenHeadlessPrintUsesUpdatedState)
{
    ::jxx::initialize();
    auto component = ::jxx::NEW<FilledComponent>();
    component->setSize(4, 4);
    auto red = ::jxx::NEW<::jxx::awt::Color>(255, 0, 0);
    ::jxx::ext::swing::SwingUtilities::invokeAndWait(
        ::jxx::NEW<UpdateTask>(component, red));

    auto image = ::jxx::NEW<::jxx::awt::image::BufferedImage>(
        4, 4, ::jxx::awt::image::BufferedImage::TYPE_INT_ARGB);
    auto graphics = image->getGraphics();
    component->printAll(graphics);
    graphics->dispose();
    EXPECT_EQ(static_cast<std::uint32_t>(image->getRGB(2, 2)),
        static_cast<std::uint32_t>(red->getRGB()));
}

} // namespace
