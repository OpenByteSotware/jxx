#include "awt/jxx.awt.Dimension.h"

#include <cmath>
#include <cstdint>
#include <sstream>

#include "io/jxx.io.ObjectInputStream.h"
#include "io/jxx.io.ObjectOutputStream.h"
#include "lang/jxx.lang.NullPointerException.h"

namespace jxx::awt {

Dimension::Dimension(
    ::jxx::lang::jint widthValue,
    ::jxx::lang::jint heightValue)
    : width(widthValue),
      height(heightValue) {
}

Dimension::Dimension(const ::jxx::Ptr<Dimension>& dimension) {
    if (dimension == nullptr) {
        throw ::jxx::lang::NullPointerException();
    }
    width = dimension->width;
    height = dimension->height;
}

::jxx::lang::jdouble Dimension::getWidth() const {
    return static_cast<::jxx::lang::jdouble>(width);
}

::jxx::lang::jdouble Dimension::getHeight() const {
    return static_cast<::jxx::lang::jdouble>(height);
}

::jxx::Ptr<Dimension> Dimension::getSize() const {
    return ::jxx::NEW<Dimension>(width, height);
}

void Dimension::setSize(const ::jxx::Ptr<Dimension>& dimension) {
    if (dimension == nullptr) {
        throw ::jxx::lang::NullPointerException();
    }
    setSize(dimension->width, dimension->height);
}

void Dimension::setSize(
    ::jxx::lang::jint widthValue,
    ::jxx::lang::jint heightValue) {
    width = widthValue;
    height = heightValue;
}

void Dimension::setSize(
    ::jxx::lang::jdouble widthValue,
    ::jxx::lang::jdouble heightValue) {
    width = static_cast<::jxx::lang::jint>(std::ceil(widthValue));
    height = static_cast<::jxx::lang::jint>(std::ceil(heightValue));
}

::jxx::lang::jbool Dimension::equals(
    const ::jxx::Ptr<::jxx::lang::Object>& object) const {
    const auto dimension = ::jxx::CAST<Dimension>(object);
    return dimension != nullptr &&
        dimension->width == width &&
        dimension->height == height;
}

::jxx::lang::jint Dimension::hashCode() const {
    const auto sum = static_cast<std::int64_t>(width) +
        static_cast<std::int64_t>(height);
    return static_cast<::jxx::lang::jint>(
        (sum * (sum + 1LL)) / 2LL + width);
}

::jxx::Ptr<::jxx::lang::String> Dimension::toString() const {
    std::ostringstream stream;
    stream << "jxx.awt.Dimension[width=" << width
           << ",height=" << height << "]";
    return ::jxx::NEW<::jxx::lang::String>(stream.str());
}

void Dimension::writeObject(
    const ::jxx::Ptr<::jxx::io::ObjectOutputStream>& out) {
    if (out == nullptr) {
        throw ::jxx::lang::NullPointerException();
    }
    out->writeInt(width);
    out->writeInt(height);
}

void Dimension::readObject(
    const ::jxx::Ptr<::jxx::io::ObjectInputStream>& in) {
    if (in == nullptr) {
        throw ::jxx::lang::NullPointerException();
    }
    width = in->readInt();
    height = in->readInt();
}

void Dimension::readObjectNoData() {
    width = 0;
    height = 0;
}

::jxx::Ptr<::jxx::lang::Object> Dimension::cloneImpl() const {
    return ::jxx::CAST<::jxx::lang::Object>(
        ::jxx::NEW<Dimension>(width, height));
}

} // namespace jxx::awt
