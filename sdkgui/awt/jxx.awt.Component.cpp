#include "awt/jxx.awt.Component.h"

#include "awt/jxx.awt.Container.h"
#include "gui/internal/jxx.gui.internal.NativeComponent.h"

namespace jxx::awt
{
    Component::Component() = default;

    Component::~Component()
    {
        if (nativeComponent_ != nullptr) nativeComponent_->destroy();
    }

    ::jxx::lang::jbool Component::isVisible() const { return visible_; }
    void Component::setVisible(::jxx::lang::jbool value) { visible_=value; if(nativeComponent_) nativeComponent_->setVisible(value); }
    ::jxx::lang::jbool Component::isEnabled() const { return enabled_; }
    void Component::setEnabled(::jxx::lang::jbool value) { enabled_=value; if(nativeComponent_) nativeComponent_->setEnabled(value); }
    ::jxx::lang::jint Component::getX() const { return x_; }
    ::jxx::lang::jint Component::getY() const { return y_; }
    ::jxx::lang::jint Component::getWidth() const { return width_; }
    ::jxx::lang::jint Component::getHeight() const { return height_; }

    ::jxx::Ptr<Point> Component::getLocation() const { return ::jxx::NEW<Point>(x_,y_); }
    void Component::setLocation(const ::jxx::Ptr<Point>& value) { if(value) setLocation(value->x,value->y); }
    void Component::setLocation(::jxx::lang::jint x,::jxx::lang::jint y) { x_=x; y_=y; if(nativeComponent_) nativeComponent_->setBounds(x_,y_,width_,height_); invalidate(); }
    ::jxx::Ptr<Dimension> Component::getSize() const { return ::jxx::NEW<Dimension>(width_,height_); }
    void Component::setSize(const ::jxx::Ptr<Dimension>& value) { if(value) setSize(value->width,value->height); }
    void Component::setSize(::jxx::lang::jint width,::jxx::lang::jint height) { width_=width; height_=height; if(nativeComponent_) nativeComponent_->setBounds(x_,y_,width_,height_); invalidate(); }
    ::jxx::Ptr<Rectangle> Component::getBounds() const { return ::jxx::NEW<Rectangle>(x_,y_,width_,height_); }
    void Component::setBounds(const ::jxx::Ptr<Rectangle>& value) { if(value) setBounds(value->x,value->y,value->width,value->height); }
    void Component::setBounds(::jxx::lang::jint x,::jxx::lang::jint y,::jxx::lang::jint width,::jxx::lang::jint height) { x_=x; y_=y; width_=width; height_=height; if(nativeComponent_) nativeComponent_->setBounds(x,y,width,height); invalidate(); }

    ::jxx::Ptr<Color> Component::getForeground() const { return foreground_; }
    void Component::setForeground(const ::jxx::Ptr<Color>& value) { foreground_=value; if(nativeComponent_) nativeComponent_->setForeground(value); }
    ::jxx::Ptr<Color> Component::getBackground() const { return background_; }
    void Component::setBackground(const ::jxx::Ptr<Color>& value) { background_=value; if(nativeComponent_) nativeComponent_->setBackground(value); }
    ::jxx::Ptr<Font> Component::getFont() const { return font_; }
    void Component::setFont(const ::jxx::Ptr<Font>& value) { font_=value; if(nativeComponent_) nativeComponent_->setFont(value); invalidate(); }
    ::jxx::Ptr<::jxx::lang::String> Component::getName() const { return name_; }
    void Component::setName(const ::jxx::Ptr<::jxx::lang::String>& value) { name_=value; }
    ::jxx::Ptr<Container> Component::getParent() const { return parent_.lock(); }
    void Component::setParentInternal(const ::jxx::Ptr<Container>& value) { parent_=value; }

    void Component::setNativeComponentInternal(
        const ::jxx::Ptr<::jxx::gui::internal::NativeComponent>& value)
    {
        nativeComponent_ = value;
        if (nativeComponent_ == nullptr) return;

        nativeComponent_->setBounds(x_, y_, width_, height_);
        nativeComponent_->setVisible(visible_);
        nativeComponent_->setEnabled(enabled_);
        if (foreground_ != nullptr) nativeComponent_->setForeground(foreground_);
        if (background_ != nullptr) nativeComponent_->setBackground(background_);
        if (font_ != nullptr) nativeComponent_->setFont(font_);
    }

    void Component::invalidate() { valid_=false; }
    void Component::validate() { valid_=true; }
    ::jxx::lang::jbool Component::isValid() const { return valid_; }
}
