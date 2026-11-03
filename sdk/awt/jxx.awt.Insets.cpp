#include "awt/jxx.awt.Insets.h"

#include <cstdint>
#include <sstream>

#include "io/jxx.io.ObjectInputStream.h"
#include "io/jxx.io.ObjectOutputStream.h"
#include "lang/jxx.lang.NullPointerException.h"

namespace jxx::awt {

Insets::Insets(::jxx::lang::jint topValue,
               ::jxx::lang::jint leftValue,
               ::jxx::lang::jint bottomValue,
               ::jxx::lang::jint rightValue)
    : top(topValue),
      left(leftValue),
      bottom(bottomValue),
      right(rightValue) {
}

void Insets::set(::jxx::lang::jint topValue,
                 ::jxx::lang::jint leftValue,
                 ::jxx::lang::jint bottomValue,
                 ::jxx::lang::jint rightValue) {
    top = topValue;
    left = leftValue;
    bottom = bottomValue;
    right = rightValue;
}

::jxx::Ptr<::jxx::lang::Object> Insets::clone() const {
    return ::jxx::CAST<::jxx::lang::Object>(
        ::jxx::NEW<Insets>(top, left, bottom, right));
}

::jxx::lang::jbool Insets::equals(
    const ::jxx::Ptr<::jxx::lang::Object>& object) const {
    const auto value = ::jxx::CAST<Insets>(object);
    return value != nullptr && value->top == top && value->left == left &&
        value->bottom == bottom && value->right == right;
}

::jxx::lang::jint Insets::hashCode() const {
    std::uint32_t value = static_cast<std::uint32_t>(top);
    value = value * 31U + static_cast<std::uint32_t>(left);
    value = value * 31U + static_cast<std::uint32_t>(bottom);
    value = value * 31U + static_cast<std::uint32_t>(right);
    return static_cast<::jxx::lang::jint>(value);
}

::jxx::Ptr<::jxx::lang::String> Insets::toString() const {
    std::ostringstream stream;
    stream << "jxx.awt.Insets[top=" << top
           << ",left=" << left
           << ",bottom=" << bottom
           << ",right=" << right << "]";
    return ::jxx::NEW<::jxx::lang::String>(stream.str());
}

void Insets::writeObject(
    const ::jxx::Ptr<::jxx::io::ObjectOutputStream>& output) {
    if (output == nullptr) {
        throw ::jxx::lang::NullPointerException();
    }
    output->writeInt(top);
    output->writeInt(left);
    output->writeInt(bottom);
    output->writeInt(right);
}

void Insets::readObject(
    const ::jxx::Ptr<::jxx::io::ObjectInputStream>& input) {
    if (input == nullptr) {
        throw ::jxx::lang::NullPointerException();
    }
    top = input->readInt();
    left = input->readInt();
    bottom = input->readInt();
    right = input->readInt();
}

void Insets::readObjectNoData() {
    top = 0;
    left = 0;
    bottom = 0;
    right = 0;
}

} // namespace jxx::awt
