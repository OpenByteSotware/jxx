#pragma once

#include "io/jxx.io.DataOutputStream.h"
#include "io/jxx.io.ObjectOutput.h"

namespace jxx::lang {
class Object;
}

namespace jxx::io {

class ObjectOutputStream
    : public ::jxx::lang::ClassBase<
          ObjectOutputStream,
          DataOutputStream,
          ObjectOutput> {
public:
    using JxxSuper = DataOutputStream;

    using Super =
        ::jxx::lang::ClassBase<
            ObjectOutputStream,
            JxxSuper,
            ObjectOutput>;

    explicit ObjectOutputStream(
        const ::jxx::Ptr<OutputStream>& output);

    ~ObjectOutputStream() override;

    void writeObject(
        const ::jxx::Ptr<::jxx::lang::Object>& object) override;

    virtual void writeUnshared(
        const ::jxx::Ptr<::jxx::lang::Object>& object);

protected:
    ObjectOutputStream();
};

} // namespace jxx::io
