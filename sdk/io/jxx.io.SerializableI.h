#pragma once

#include "lang/jxx.lang.ClassInfo.h"
#include "io/jxx.io.Serializable.h"

namespace jxx::io
{

    class ObjectInputStream;
    class ObjectOutputStream;

    class SerializableI : public jxx::lang::InterfaceBase<SerializableI, Serializable>
    {
    public:
        ~SerializableI() override = default;

        virtual void writeObject(const jxx::Ptr<ObjectOutputStream>& out) = 0;

        virtual void readObject(const jxx::Ptr<ObjectInputStream>& in) = 0;

        virtual void readObjectNoData() = 0;
    };

} // namespace jxx::io