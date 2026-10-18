#pragma once
#include "awt/jxx.awt.Dimension.h"
#include "awt/jxx.awt.Point.h"
namespace jxx::awt
{
	class Rectangle : public ::jxx::lang::ClassBase<Rectangle, ::jxx::lang::Object>
	{
	public: using JxxSuper = ::jxx::lang::Object; using Super = ::jxx::lang::ClassBase<Rectangle, JxxSuper>; ::jxx::lang::jint x = 0, y = 0, width = 0, height = 0; Rectangle() = default; Rectangle(::jxx::lang::jint x, ::jxx::lang::jint y, ::jxx::lang::jint width,
		::jxx::lang::jint height); 
		  explicit Rectangle(const ::jxx::Ptr<Dimension>& dimension);
		  Rectangle(const ::jxx::Ptr<Point>& point, const ::jxx::Ptr<Dimension>& dimension);
		  ::jxx::Ptr<Point> getLocation() const; ::jxx::Ptr<Dimension> getSize() const;
		  void setBounds(::jxx::lang::jint x, ::jxx::lang::jint y, ::jxx::lang::jint width,
			  ::jxx::lang::jint height); ::jxx::lang::jbool contains(::jxx::lang::jint x, 
				  ::jxx::lang::jint y) const;
		  ::jxx::lang::jbool intersects(const ::jxx::Ptr<Rectangle>& rectangle) const;
		  ::jxx::lang::jbool isEmpty() const;
		  ::jxx::lang::jbool equals(const ::jxx::Ptr<::jxx::lang::Object>& object) const override;
		  ::jxx::lang::jint hashCode() const override; 
		  ::jxx::Ptr<::jxx::lang::String> toString() const override;
	};
}
