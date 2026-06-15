#pragma once

#include <memory>
#include <string>
#include <vector>

#include "lang/jxx_types.h"
#include "nio/jxx.nio.ByteBuffer.h"

namespace jxx::nio
{
    class MappedByteBuffer final : public ByteBuffer
    {
    public:
        ~MappedByteBuffer() override = default;

    public:
        static jxx::Ptr<MappedByteBuffer> mapFile(
            const std::string& path,
            jxx::lang::jlong fileOffset,
            jxx::lang::jint size,
            jxx::lang::jbool readOnly,
            jxx::Ptr<ByteOrder> order);

        jxx::lang::jbool isLoaded() const;
        jxx::Ptr<MappedByteBuffer> load();
        jxx::Ptr<MappedByteBuffer> force();

        jxx::Ptr<MappedByteBuffer> sliceMapped() const;
        jxx::Ptr<MappedByteBuffer> duplicateMapped() const;
        jxx::Ptr<MappedByteBuffer> asReadOnlyMappedBuffer() const;

    private:
        MappedByteBuffer(std::shared_ptr<std::vector<jxx::lang::jbyte>> storage,
                         jxx::lang::jint offset,
                         jxx::lang::jint capacity,
                         jxx::lang::jbool readOnly,
                         const std::string& path,
                         jxx::lang::jlong fileOffset,
                         jxx::Ptr<ByteOrder> order);

        jxx::Ptr<MappedByteBuffer> self_Mapped_();

    private:
        std::string path_;
        jxx::lang::jlong fileOffset_ = 0;
        jxx::lang::jbool loaded_ = true;
    };
}
