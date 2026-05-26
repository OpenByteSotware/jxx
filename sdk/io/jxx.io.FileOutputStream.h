#pragma once

#include "jxx.io.OutputStream.h"
#include "jxx.io.IOException.h"


#include <fstream>

namespace jxx { namespace lang {
    class String;
}
}

namespace jxx::io {

class FileOutputStream final : public OutputStream {
public:
    explicit FileOutputStream(jxx::Ptr<jxx::lang::String> path);
    FileOutputStream(jxx::Ptr<jxx::lang::String> path, jxx::lang::jbool append);

    virtual void write(jxx::lang::jint b) override;
    virtual void write(const jxx::lang::ByteArray b, jxx::lang::jint off, jxx::lang::jint len) override;

    virtual void flush() override;
    virtual void close() override;

private:
    std::ofstream f_;
};

} // namespace jxx::io
