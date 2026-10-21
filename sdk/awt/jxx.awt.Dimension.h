#pragma once

#include "awt/geom/jxx.awt.geom.Dimension2D.h"
#include "io/jxx.io.SerializableI.h"
#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.String.h"
#include "lang/jxx_types.h"

namespace jxx::io {
class ObjectInputStream;
class ObjectOutputStream;
}

namespace jxx::awt {

class Dimension
    : public ::jxx::lang::ClassBase<
          Dimension,
          ::jxx::awt::geom::Dimension2D,
          ::jxx::io::SerializableI> {
public:
    using JxxSuper = ::jxx::awt::geom::Dimension2D;
    using Super = ::jxx::lang::ClassBase<
        Dimension,
        JxxSuper,
        ::jxx::io::SerializableI>;

    ::jxx::lang::jint width = 0;
    ::jxx::lang::jint height = 0;

    Dimension() = default;
    Dimension(
        ::jxx::lang::jint width,
        ::jxx::lang::jint height);
    explicit Dimension(const ::jxx::Ptr<Dimension>& dimension);

    ::jxx::lang::jdouble getWidth() const override;
    ::jxx::lang::jdouble getHeight() const override;
    ::jxx::Ptr<Dimension> getSize() const;

    void setSize(const ::jxx::Ptr<Dimension>& dimension);
    void setSize(
        ::jxx::lang::jint width,
        ::jxx::lang::jint height);
    void setSize(
        ::jxx::lang::jdouble width,
        ::jxx::lang::jdouble height) override;

    ::jxx::lang::jbool equals(
        const ::jxx::Ptr<::jxx::lang::Object>& object) const override;
    ::jxx::lang::jint hashCode() const override;
    ::jxx::Ptr<::jxx::lang::String> toString() const override;

    void writeObject(
        const ::jxx::Ptr<::jxx::io::ObjectOutputStream>& out) override;
    void readObject(
        const ::jxx::Ptr<::jxx::io::ObjectInputStream>& in) override;
    void readObjectNoData() override;

protected:
    ::jxx::Ptr<::jxx::lang::Object> cloneImpl() const override;
};

} // namespace jxx::awt
