#pragma once
#include "jxx.io.Serializable.h"
namespace jxx::io {
class ObjectOutputStream; class ObjectInputStream;
struct Externalizable : public Serializable {
    virtual ~Externalizable() = default;
    virtual void writeExternal(ObjectOutputStream& out) const = 0;
    virtual void readExternal(ObjectInputStream& in) = 0;
};
}