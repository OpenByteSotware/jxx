#include "lang/jxx.lang.Byte.h"
#include "lang/jxx.lang.Short.h"
#include "lang/jxx.lang.Integer.h"
#include "lang/jxx.lang.Long.h"
#include "lang/jxx.lang.Float.h"
#include "lang/jxx.lang.Double.h"

#include "io/jxx.io.ObjectInputStream.h"
#include "io/jxx.io.ObjectOutputStream.h"
#include "lang/jxx.lang.NullPointerException.h"

namespace jxx::lang {

void Byte::writeObject(const ::jxx::Ptr<::jxx::io::ObjectOutputStream>& out) {
    if (out == nullptr) throw NullPointerException();
    out->writeByte(value_);
}
void Byte::readObject(const ::jxx::Ptr<::jxx::io::ObjectInputStream>& in) {
    if (in == nullptr) throw NullPointerException();
    value_ = in->readByte();
}
void Byte::readObjectNoData() { value_ = 0; }

void Short::writeObject(const ::jxx::Ptr<::jxx::io::ObjectOutputStream>& out) {
    if (out == nullptr) throw NullPointerException();
    out->writeShort(value_);
}
void Short::readObject(const ::jxx::Ptr<::jxx::io::ObjectInputStream>& in) {
    if (in == nullptr) throw NullPointerException();
    value_ = in->readShort();
}
void Short::readObjectNoData() { value_ = 0; }

void Integer::writeObject(const ::jxx::Ptr<::jxx::io::ObjectOutputStream>& out) {
    if (out == nullptr) throw NullPointerException();
    out->writeInt(value_);
}
void Integer::readObject(const ::jxx::Ptr<::jxx::io::ObjectInputStream>& in) {
    if (in == nullptr) throw NullPointerException();
    value_ = in->readInt();
}
void Integer::readObjectNoData() { value_ = 0; }

void Long::writeObject(const ::jxx::Ptr<::jxx::io::ObjectOutputStream>& out) {
    if (out == nullptr) throw NullPointerException();
    out->writeLong(value_);
}
void Long::readObject(const ::jxx::Ptr<::jxx::io::ObjectInputStream>& in) {
    if (in == nullptr) throw NullPointerException();
    value_ = in->readLong();
}
void Long::readObjectNoData() { value_ = 0; }

void Float::writeObject(const ::jxx::Ptr<::jxx::io::ObjectOutputStream>& out) {
    if (out == nullptr) throw NullPointerException();
    out->writeFloat(value_);
}
void Float::readObject(const ::jxx::Ptr<::jxx::io::ObjectInputStream>& in) {
    if (in == nullptr) throw NullPointerException();
    value_ = in->readFloat();
}
void Float::readObjectNoData() { value_ = 0.0F; }

void Double::writeObject(const ::jxx::Ptr<::jxx::io::ObjectOutputStream>& out) {
    if (out == nullptr) throw NullPointerException();
    out->writeDouble(value_);
}
void Double::readObject(const ::jxx::Ptr<::jxx::io::ObjectInputStream>& in) {
    if (in == nullptr) throw NullPointerException();
    value_ = in->readDouble();
}
void Double::readObjectNoData() { value_ = 0.0; }

} // namespace jxx::lang
