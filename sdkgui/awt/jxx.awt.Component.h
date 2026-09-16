#pragma once
#include "awt/jxx.awt.Color.h"
#include "awt/jxx.awt.Dimension.h"
#include "awt/jxx.awt.Font.h"
#include "awt/jxx.awt.Point.h"
#include "awt/jxx.awt.Rectangle.h"
namespace jxx::gui::internal { class NativeComponent; class WxFramePeer; }
namespace jxx::awt { class Container;
class Component : public ::jxx::lang::ClassBase<Component,::jxx::lang::Object> {
public:
 using JxxSuper=::jxx::lang::Object; using Super=::jxx::lang::ClassBase<Component,JxxSuper>; virtual ~Component()=0;
 virtual ::jxx::lang::jbool isVisible() const; virtual void setVisible(::jxx::lang::jbool visible); virtual ::jxx::lang::jbool isEnabled() const; virtual void setEnabled(::jxx::lang::jbool enabled);
 virtual ::jxx::lang::jint getX() const; virtual ::jxx::lang::jint getY() const; virtual ::jxx::lang::jint getWidth() const; virtual ::jxx::lang::jint getHeight() const;
 virtual ::jxx::Ptr<Point> getLocation() const; virtual void setLocation(const ::jxx::Ptr<Point>& point); virtual void setLocation(::jxx::lang::jint x,::jxx::lang::jint y);
 virtual ::jxx::Ptr<Dimension> getSize() const; virtual void setSize(const ::jxx::Ptr<Dimension>& dimension); virtual void setSize(::jxx::lang::jint width,::jxx::lang::jint height);
 virtual ::jxx::Ptr<Rectangle> getBounds() const; virtual void setBounds(const ::jxx::Ptr<Rectangle>& rectangle); virtual void setBounds(::jxx::lang::jint x,::jxx::lang::jint y,::jxx::lang::jint width,::jxx::lang::jint height);
 virtual ::jxx::Ptr<Color> getForeground() const; virtual void setForeground(const ::jxx::Ptr<Color>& color); virtual ::jxx::Ptr<Color> getBackground() const; virtual void setBackground(const ::jxx::Ptr<Color>& color);
 virtual ::jxx::Ptr<Font> getFont() const; virtual void setFont(const ::jxx::Ptr<Font>& font); virtual ::jxx::Ptr<::jxx::lang::String> getName() const; virtual void setName(const ::jxx::Ptr<::jxx::lang::String>& name); virtual ::jxx::Ptr<Container> getParent() const;
 virtual void invalidate(); virtual void validate(); virtual ::jxx::lang::jbool isValid() const;
protected: Component(); ::jxx::Ptr<::jxx::gui::internal::NativeComponent> nativeComponent_;
private:
 friend class Container; friend class ::jxx::gui::internal::WxFramePeer;
 void setParentInternal(const ::jxx::Ptr<Container>& parent); void setNativeComponentInternal(const ::jxx::Ptr<::jxx::gui::internal::NativeComponent>& peer);
 ::jxx::lang::jint x_=0,y_=0,width_=0,height_=0; ::jxx::lang::jbool visible_=true,enabled_=true,valid_=false; ::jxx::Ptr<Color> foreground_; ::jxx::Ptr<Color> background_; ::jxx::Ptr<Font> font_; ::jxx::Ptr<::jxx::lang::String> name_; std::weak_ptr<Container> parent_;
}; }
