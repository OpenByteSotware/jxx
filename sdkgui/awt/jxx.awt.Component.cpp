#include "awt/jxx.awt.Component.h"
#include "awt/jxx.awt.Container.h"
namespace jxx::awt
{
	Component::Component() = default; Component::~Component() = default;
	::jxx::lang::jbool Component::isVisible() const
	{
		return visible_;
	} void Component::setVisible(::jxx::lang::jbool v)
	{
		visible_ = v;
	} ::jxx::lang::jbool Component::isEnabled() const
	{
		return enabled_;
	} void Component::setEnabled(::jxx::lang::jbool e)
	{
		enabled_ = e;
	}
	::jxx::lang::jint Component::getX() const
	{
		return x_;
	} ::jxx::lang::jint Component::getY() const
	{
		return y_;
	} ::jxx::lang::jint Component::getWidth() const
	{
		return width_;
	} ::jxx::lang::jint Component::getHeight() const
	{
		return height_;
	}
	::jxx::Ptr<Point> Component::getLocation() const
	{
		return ::jxx::NEW<Point>(x_, y_);
	} void Component::setLocation(const ::jxx::Ptr<Point>& p)
	{
		if (p)setLocation(p->x, p->y);
	} void Component::setLocation(::jxx::lang::jint x, ::jxx::lang::jint y)
	{
		x_ = x; y_ = y; invalidate();
	}
	::jxx::Ptr<Dimension> Component::getSize() const
	{
		return ::jxx::NEW<Dimension>(width_, height_);
	} void Component::setSize(const ::jxx::Ptr<Dimension>& d)
	{
		if (d)setSize(d->width, d->height);
	} void Component::setSize(::jxx::lang::jint w, ::jxx::lang::jint h)
	{
		width_ = w; height_ = h; invalidate();
	}
	::jxx::Ptr<Rectangle> Component::getBounds() const
	{
		return ::jxx::NEW<Rectangle>(x_, y_, width_, height_);
	} void Component::setBounds(const ::jxx::Ptr<Rectangle>& r)
	{
		if (r)setBounds(r->x, r->y, r->width, r->height);
	} void Component::setBounds(::jxx::lang::jint x, ::jxx::lang::jint y, ::jxx::lang::jint w, ::jxx::lang::jint h)
	{
		x_ = x; y_ = y; width_ = w; height_ = h; invalidate();
	}
	::jxx::Ptr<Color> Component::getForeground() const
	{
		return foreground_;
	} void Component::setForeground(const ::jxx::Ptr<Color>& c)
	{
		foreground_ = c;
	} ::jxx::Ptr<Color> Component::getBackground() const
	{
		return background_;
	} void Component::setBackground(const ::jxx::Ptr<Color>& c)
	{
		background_ = c;
	} ::jxx::Ptr<Font> Component::getFont() const
	{
		return font_;
	} void Component::setFont(const ::jxx::Ptr<Font>& f)
	{
		font_ = f; invalidate();
	} ::jxx::Ptr<::jxx::lang::String> Component::getName() const
	{
		return name_;
	} void Component::setName(const ::jxx::Ptr<::jxx::lang::String>& n)
	{
		name_ = n;
	} ::jxx::Ptr<Container> Component::getParent() const
	{
		return parent_.lock();
	} void Component::setParentInternal(const ::jxx::Ptr<Container>& p)
	{
		parent_ = p;
	} void Component::invalidate()
	{
		valid_ = false;
	} void Component::validate()
	{
		valid_ = true;
	} ::jxx::lang::jbool Component::isValid() const
	{
		return valid_;
	}
}
