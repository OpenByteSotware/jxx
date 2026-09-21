#pragma once
#include <vector>
#include "awt/jxx.awt.Color.h"
#include "awt/jxx.awt.ComponentOrientation.h"
#include "awt/jxx.awt.Dimension.h"
#include "awt/jxx.awt.Font.h"
#include "awt/jxx.awt.Point.h"
#include "awt/jxx.awt.Rectangle.h"
namespace jxx::awt::event { class ComponentEvent; class ComponentListener; class FocusEvent; class FocusListener; class KeyEvent; class KeyListener; class MouseEvent; class MouseListener; class MouseMotionListener; class MouseWheelEvent; class MouseWheelListener; class HierarchyEvent; class HierarchyListener; class HierarchyBoundsListener; }
namespace jxx::gui::internal { class NativeComponent; class WxFramePeer; class WxDialogPeer; }
namespace jxx::awt {
class Container; class DefaultKeyboardFocusManager;
class Component : public ::jxx::lang::ClassBase<Component, ::jxx::lang::Object> {
public:
 using JxxSuper=::jxx::lang::Object; using Super=::jxx::lang::ClassBase<Component,JxxSuper>; virtual ~Component()=0;
 virtual ::jxx::lang::jbool isVisible() const; virtual void setVisible(::jxx::lang::jbool visible); virtual ::jxx::lang::jbool isEnabled() const; virtual void setEnabled(::jxx::lang::jbool enabled); virtual void setFocusable(::jxx::lang::jbool focusable); virtual ::jxx::lang::jbool isFocusable() const; virtual ::jxx::lang::jbool isFocusTraversable() const; virtual ::jxx::lang::jbool requestFocusInWindow();
 virtual ::jxx::lang::jint getX() const; virtual ::jxx::lang::jint getY() const; virtual ::jxx::lang::jint getWidth() const; virtual ::jxx::lang::jint getHeight() const;
 virtual ::jxx::Ptr<Point> getLocation() const; virtual void setLocation(const ::jxx::Ptr<Point>& point); virtual void setLocation(::jxx::lang::jint x,::jxx::lang::jint y);
 virtual ::jxx::Ptr<Dimension> getSize() const; virtual void setSize(const ::jxx::Ptr<Dimension>& dimension); virtual void setSize(::jxx::lang::jint width,::jxx::lang::jint height);
 virtual ::jxx::Ptr<Rectangle> getBounds() const; virtual void setBounds(const ::jxx::Ptr<Rectangle>& rectangle); virtual void setBounds(::jxx::lang::jint x,::jxx::lang::jint y,::jxx::lang::jint width,::jxx::lang::jint height);
 virtual ::jxx::Ptr<Color> getForeground() const; virtual void setForeground(const ::jxx::Ptr<Color>& color); virtual ::jxx::Ptr<Color> getBackground() const; virtual void setBackground(const ::jxx::Ptr<Color>& color); virtual ::jxx::Ptr<Font> getFont() const; virtual void setFont(const ::jxx::Ptr<Font>& font);
 virtual ::jxx::Ptr<::jxx::lang::String> getName() const; virtual void setName(const ::jxx::Ptr<::jxx::lang::String>& name); virtual ::jxx::Ptr<Container> getParent() const;
 virtual ::jxx::Ptr<ComponentOrientation> getComponentOrientation() const; virtual void setComponentOrientation(const ::jxx::Ptr<ComponentOrientation>& orientation); virtual void applyComponentOrientation(const ::jxx::Ptr<ComponentOrientation>& orientation);
 virtual void addComponentListener(const ::jxx::Ptr<::jxx::awt::event::ComponentListener>& listener); virtual void removeComponentListener(const ::jxx::Ptr<::jxx::awt::event::ComponentListener>& listener);
 virtual void addFocusListener(const ::jxx::Ptr<::jxx::awt::event::FocusListener>& listener); virtual void removeFocusListener(const ::jxx::Ptr<::jxx::awt::event::FocusListener>& listener); virtual void addKeyListener(const ::jxx::Ptr<::jxx::awt::event::KeyListener>& listener); virtual void removeKeyListener(const ::jxx::Ptr<::jxx::awt::event::KeyListener>& listener);
 virtual void addMouseListener(const ::jxx::Ptr<::jxx::awt::event::MouseListener>&); virtual void removeMouseListener(const ::jxx::Ptr<::jxx::awt::event::MouseListener>&); virtual void addMouseMotionListener(const ::jxx::Ptr<::jxx::awt::event::MouseMotionListener>&); virtual void removeMouseMotionListener(const ::jxx::Ptr<::jxx::awt::event::MouseMotionListener>&); virtual void addMouseWheelListener(const ::jxx::Ptr<::jxx::awt::event::MouseWheelListener>& listener); virtual void removeMouseWheelListener(const ::jxx::Ptr<::jxx::awt::event::MouseWheelListener>& listener);
 virtual void addHierarchyListener(const ::jxx::Ptr<::jxx::awt::event::HierarchyListener>& listener); virtual void removeHierarchyListener(const ::jxx::Ptr<::jxx::awt::event::HierarchyListener>& listener); virtual void addHierarchyBoundsListener(const ::jxx::Ptr<::jxx::awt::event::HierarchyBoundsListener>& listener); virtual void removeHierarchyBoundsListener(const ::jxx::Ptr<::jxx::awt::event::HierarchyBoundsListener>& listener);
 virtual void requestFocus(); virtual ::jxx::lang::jbool isFocusOwner() const; virtual void invalidate(); virtual void validate(); virtual ::jxx::lang::jbool isValid() const;
protected:
 Component(); virtual void processComponentEvent(const ::jxx::Ptr<::jxx::awt::event::ComponentEvent>& event); virtual void processFocusEvent(const ::jxx::Ptr<::jxx::awt::event::FocusEvent>& event); virtual void processMouseEvent(const ::jxx::Ptr<::jxx::awt::event::MouseEvent>& event); virtual void processMouseWheelEvent(const ::jxx::Ptr<::jxx::awt::event::MouseWheelEvent>& event); virtual void processHierarchyEvent(const ::jxx::Ptr<::jxx::awt::event::HierarchyEvent>& event); virtual void processHierarchyBoundsEvent(const ::jxx::Ptr<::jxx::awt::event::HierarchyEvent>& event); virtual void processKeyEvent(const ::jxx::Ptr<::jxx::awt::event::KeyEvent>& event); ::jxx::Ptr<::jxx::gui::internal::NativeComponent> nativeComponent_;
private:
 friend class Container; friend class DefaultKeyboardFocusManager; friend class ::jxx::gui::internal::WxFramePeer; friend class ::jxx::gui::internal::WxDialogPeer;
 void setParentInternal(const ::jxx::Ptr<Container>& parent); void setNativeComponentInternal(const ::jxx::Ptr<::jxx::gui::internal::NativeComponent>& peer); void fireComponentEvent(::jxx::lang::jint id); void fireHierarchyEvent(::jxx::lang::jint id,const ::jxx::Ptr<Component>& changed,const ::jxx::Ptr<Container>& changedParent,::jxx::lang::jlong flags);
 ::jxx::lang::jint x_=0,y_=0,width_=0,height_=0; ::jxx::lang::jbool visible_=true,enabled_=true,focusable_=true,valid_=false; ::jxx::Ptr<Color> foreground_; ::jxx::Ptr<Color> background_; ::jxx::Ptr<Font> font_; ::jxx::Ptr<::jxx::lang::String> name_; ::jxx::Ptr<ComponentOrientation> componentOrientation_=ComponentOrientation::UNKNOWN; std::weak_ptr<Container> parent_;
 std::vector<::jxx::Ptr<::jxx::awt::event::ComponentListener>> componentListeners_; std::vector<::jxx::Ptr<::jxx::awt::event::FocusListener>> focusListeners_; std::vector<::jxx::Ptr<::jxx::awt::event::KeyListener>> keyListeners_; std::vector<::jxx::Ptr<::jxx::awt::event::MouseListener>> mouseListeners_; std::vector<::jxx::Ptr<::jxx::awt::event::MouseMotionListener>> mouseMotionListeners_; std::vector<::jxx::Ptr<::jxx::awt::event::MouseWheelListener>> mouseWheelListeners_; std::vector<::jxx::Ptr<::jxx::awt::event::HierarchyListener>> hierarchyListeners_; std::vector<::jxx::Ptr<::jxx::awt::event::HierarchyBoundsListener>> hierarchyBoundsListeners_;
}; }
