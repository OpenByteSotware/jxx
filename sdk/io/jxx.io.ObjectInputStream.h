#pragma once

#include "io/jxx.io.DataInputStream.h"
#include "io/jxx.io.ObjectInput.h"

namespace jxx::lang {
class Object;
}

namespace jxx::io {

class ObjectInputStream
    : public ::jxx::lang::ClassBase<
          ObjectInputStream,
          DataInputStream,
          ObjectInput> {
public:
    using JxxSuper = DataInputStream;

    using Super =
        ::jxx::lang::ClassBase<
            ObjectInputStream,
            JxxSuper,
            ObjectInput>;

    explicit ObjectInputStream(
        const ::jxx::Ptr<InputStream>& input);

    ~ObjectInputStream() override;

    ::jxx::Ptr<::jxx::lang::Object>
    readObject() override;

    virtual ::jxx::Ptr<::jxx::lang::Object>
    readUnshared();

protected:
    ObjectInputStream();
};

} // namespace jxx::io
