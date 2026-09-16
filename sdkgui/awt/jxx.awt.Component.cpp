#include "awt/jxx.awt.Component.h"

#include <algorithm>

#include "awt/event/jxx.awt.event.ComponentEvent.h"
#include "awt/event/jxx.awt.event.ComponentListener.h"
#include "awt/event/jxx.awt.event.FocusEvent.h"
#include "awt/event/jxx.awt.event.FocusListener.h"
#include "awt/event/jxx.awt.event.KeyEvent.h"
#include "awt/event/jxx.awt.event.KeyListener.h"
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

    void Component::setVisible(::jxx::lang::jbool value)
    {
        if (visible_ == value) return;
        visible_ = value;
        if (nativeComponent_ != nullptr) nativeComponent_->setVisible(value);
        fireComponentEvent(value
            ? ::jxx::awt::event::ComponentEvent::COMPONENT_SHOWN
            : ::jxx::awt::event::ComponentEvent::COMPONENT_HIDDEN);
    }

    ::jxx::lang::jbool Component::isEnabled() const { return enabled_; }
    void Component::setEnabled(::jxx::lang::jbool value) { enabled_=value; if(nativeComponent_) nativeComponent_->setEnabled(value); }
    ::jxx::lang::jint Component::getX() const { return x_; }
    ::jxx::lang::jint Component::getY() const { return y_; }
    ::jxx::lang::jint Component::getWidth() const { return width_; }
    ::jxx::lang::jint Component::getHeight() const { return height_; }
    ::jxx::Ptr<Point> Component::getLocation() const { return ::jxx::NEW<Point>(x_,y_); }
    void Component::setLocation(const ::jxx::Ptr<Point>& value) { if(value) setLocation(value->x,value->y); }

    void Component::setLocation(::jxx::lang::jint x, ::jxx::lang::jint y)
    {
        if (x_ == x && y_ == y) return;
        x_=x; y_=y;
        if(nativeComponent_) nativeComponent_->setBounds(x_,y_,width_,height_);
        invalidate();
        fireComponentEvent(::jxx::awt::event::ComponentEvent::COMPONENT_MOVED);
    }

    ::jxx::Ptr<Dimension> Component::getSize() const { return ::jxx::NEW<Dimension>(width_,height_); }
    void Component::setSize(const ::jxx::Ptr<Dimension>& value) { if(value) setSize(value->width,value->height); }

    void Component::setSize(::jxx::lang::jint width, ::jxx::lang::jint height)
    {
        if (width_ == width && height_ == height) return;
        width_=width; height_=height;
        if(nativeComponent_) nativeComponent_->setBounds(x_,y_,width_,height_);
        invalidate();
        fireComponentEvent(::jxx::awt::event::ComponentEvent::COMPONENT_RESIZED);
    }

    ::jxx::Ptr<Rectangle> Component::getBounds() const { return ::jxx::NEW<Rectangle>(x_,y_,width_,height_); }
    void Component::setBounds(const ::jxx::Ptr<Rectangle>& value) { if(value) setBounds(value->x,value->y,value->width,value->height); }

    void Component::setBounds(::jxx::lang::jint x, ::jxx::lang::jint y,
        ::jxx::lang::jint width, ::jxx::lang::jint height)
    {
        const auto moved = x_ != x || y_ != y;
        const auto resized = width_ != width || height_ != height;
        if (!moved && !resized) return;
        x_=x; y_=y; width_=width; height_=height;
        if(nativeComponent_) nativeComponent_->setBounds(x,y,width,height);
        invalidate();
        if (moved) fireComponentEvent(::jxx::awt::event::ComponentEvent::COMPONENT_MOVED);
        if (resized) fireComponentEvent(::jxx::awt::event::ComponentEvent::COMPONENT_RESIZED);
    }

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

    void Component::addComponentListener(
        const ::jxx::Ptr<::jxx::awt::event::ComponentListener>& listener)
    {
        if (listener != nullptr &&
            std::find(componentListeners_.begin(), componentListeners_.end(), listener)
                == componentListeners_.end())
            componentListeners_.push_back(listener);
    }

    void Component::removeComponentListener(
        const ::jxx::Ptr<::jxx::awt::event::ComponentListener>& listener)
    {
        componentListeners_.erase(
            std::remove(componentListeners_.begin(), componentListeners_.end(), listener),
            componentListeners_.end());
    }

    void Component::fireComponentEvent(::jxx::lang::jint id)
    {
        if (componentListeners_.empty()) return;
        processComponentEvent(::jxx::NEW<::jxx::awt::event::ComponentEvent>(
            ::jxx::CAST<Component>(thisPtr()), id));
    }

    void Component::processComponentEvent(
        const ::jxx::Ptr<::jxx::awt::event::ComponentEvent>& event)
    {
        if (event == nullptr) return;
        const auto listeners = componentListeners_;
        for (const auto& listener : listeners)
        {
            if (listener == nullptr) continue;
            switch (event->getID())
            {
                case ::jxx::awt::event::ComponentEvent::COMPONENT_RESIZED:
                    listener->componentResized(event);
                    break;
                case ::jxx::awt::event::ComponentEvent::COMPONENT_MOVED:
                    listener->componentMoved(event);
                    break;
                case ::jxx::awt::event::ComponentEvent::COMPONENT_SHOWN:
                    listener->componentShown(event);
                    break;
                case ::jxx::awt::event::ComponentEvent::COMPONENT_HIDDEN:
                    listener->componentHidden(event);
                    break;
                default:
                    break;
            }
        }
    }

    void Component::addFocusListener(const ::jxx::Ptr<::jxx::awt::event::FocusListener>& listener){if(listener&&std::find(focusListeners_.begin(),focusListeners_.end(),listener)==focusListeners_.end())focusListeners_.push_back(listener);}
    void Component::removeFocusListener(const ::jxx::Ptr<::jxx::awt::event::FocusListener>& listener){focusListeners_.erase(std::remove(focusListeners_.begin(),focusListeners_.end(),listener),focusListeners_.end());}
    void Component::addKeyListener(const ::jxx::Ptr<::jxx::awt::event::KeyListener>& listener){if(listener&&std::find(keyListeners_.begin(),keyListeners_.end(),listener)==keyListeners_.end())keyListeners_.push_back(listener);}
    void Component::removeKeyListener(const ::jxx::Ptr<::jxx::awt::event::KeyListener>& listener){keyListeners_.erase(std::remove(keyListeners_.begin(),keyListeners_.end(),listener),keyListeners_.end());}
    void Component::processKeyEvent(const ::jxx::Ptr<::jxx::awt::event::KeyEvent>& event){const auto listeners=keyListeners_;for(const auto& listener:listeners){if(!listener)continue;if(event->getID()==::jxx::awt::event::KeyEvent::KEY_TYPED)listener->keyTyped(event);else if(event->getID()==::jxx::awt::event::KeyEvent::KEY_PRESSED)listener->keyPressed(event);else if(event->getID()==::jxx::awt::event::KeyEvent::KEY_RELEASED)listener->keyReleased(event);}}
    void Component::requestFocus(){if(nativeComponent_)nativeComponent_->requestFocus();}
    ::jxx::lang::jbool Component::isFocusOwner() const{return nativeComponent_&&nativeComponent_->hasFocus();}
    void Component::processFocusEvent(const ::jxx::Ptr<::jxx::awt::event::FocusEvent>& event){const auto listeners=focusListeners_;for(const auto& listener:listeners){if(!listener)continue;if(event->getID()==::jxx::awt::event::FocusEvent::FOCUS_GAINED)listener->focusGained(event);else if(event->getID()==::jxx::awt::event::FocusEvent::FOCUS_LOST)listener->focusLost(event);}}

    void Component::setNativeComponentInternal(
        const ::jxx::Ptr<::jxx::gui::internal::NativeComponent>& value)
    {
        if (nativeComponent_ == value) return;
        if (nativeComponent_ != nullptr) nativeComponent_->destroy();
        nativeComponent_ = value;
        if (nativeComponent_ == nullptr) return;
        nativeComponent_->setBounds(x_,y_,width_,height_);
        nativeComponent_->setVisible(visible_);
        nativeComponent_->setEnabled(enabled_);
        if(foreground_) nativeComponent_->setForeground(foreground_);
        if(background_) nativeComponent_->setBackground(background_);
        if(font_) nativeComponent_->setFont(font_);
        std::weak_ptr<Component> self=::jxx::CAST<Component>(thisPtr());
        nativeComponent_->setFocusCallback([self](::jxx::lang::jbool gained){if(auto owner=self.lock())owner->processFocusEvent(::jxx::NEW<::jxx::awt::event::FocusEvent>(owner,gained?::jxx::awt::event::FocusEvent::FOCUS_GAINED : ::jxx::awt::event::FocusEvent::FOCUS_LOST));});
        nativeComponent_->setKeyCallback([self](::jxx::lang::jint id,::jxx::lang::jint code,::jxx::lang::jchar character,::jxx::lang::jint modifiers){if(auto owner=self.lock())owner->processKeyEvent(::jxx::NEW<::jxx::awt::event::KeyEvent>(owner,id,0,modifiers,code,character));});
    }

    void Component::invalidate() { valid_=false; }
    void Component::validate() { valid_=true; }
    ::jxx::lang::jbool Component::isValid() const { return valid_; }
}
