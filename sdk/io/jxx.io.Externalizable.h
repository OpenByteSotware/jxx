#pragma once
#include "jxx.io.Serializable.h"
namespace jxx::io {
class ObjectOutputStream; class ObjectInputStream;
struct Externalizable : public Serializable {
    virtual ~Externalizable() = default;
    virtual void writeExternal(const jxx::Ptr<ObjectOutputStream>& out) const = 0;
    virtual void readExternal(const jxx::Ptr<ObjectInputStream>& in) = 0;
};
}