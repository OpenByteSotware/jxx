#pragma once
#include "lang/jxx.lang.ClassInfo.h"
#include "awt/geom/jxx.awt.geom.Point2D.h"
#include "io/jxx.io.SerializableI.h"
#include "io/jxx.io.ObjectInputStream.h"
#include "io/jxx.io.ObjectOutputStream.h"
#include "lang/jxx.lang.String.h"
#include "lang/jxx_types.h"
namespace jxx::awt {
class Point : public ::jxx::lang::ClassBase<Point, ::jxx::awt::geom::Point2D, ::jxx::io::SerializableI> {
public:
 using JxxSuper = ::jxx::awt::geom::Point2D;
 using Super = ::jxx::lang::ClassBase<Point, JxxSuper, ::jxx::io::SerializableI>;
 ::jxx::lang::jint x = 0;
 ::jxx::lang::jint y = 0;
 Point() = default;
 Point(::jxx::lang::jint x, ::jxx::lang::jint y);
 Point(const ::jxx::Ptr<Point>& point);
 ::jxx::lang::jdouble getX() const override;
 ::jxx::lang::jdouble getY() const override;
 void setLocation(::jxx::lang::jdouble x, ::jxx::lang::jdouble y) override;
 ::jxx::Ptr<Point> getLocation() const;
 void setLocation(const ::jxx::Ptr<Point>& point);
 void setLocation(::jxx::lang::jint x, ::jxx::lang::jint y);
 void move(::jxx::lang::jint x, ::jxx::lang::jint y);
 void translate(::jxx::lang::jint dx, ::jxx::lang::jint dy);
 ::jxx::lang::jbool equals(const ::jxx::Ptr<::jxx::lang::Object>& object) const override;
 ::jxx::lang::jint hashCode() const override;
 ::jxx::Ptr<::jxx::lang::String> toString() const override;
 void writeObject(const ::jxx::Ptr<::jxx::io::ObjectOutputStream>& output) override;
 void readObject(const ::jxx::Ptr<::jxx::io::ObjectInputStream>& input) override;
 void readObjectNoData() override;
};
}
