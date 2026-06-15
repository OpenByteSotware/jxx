#include "nio/jxx.nio.MappedByteBuffer.h"

#include <fstream>
#include <stdexcept>

namespace
{
    [[noreturn]] void throwIAE_(const char* msg)
    {
        throw std::invalid_argument(msg);
    }

    [[noreturn]] void throwIOE_(const char* msg)
    {
        throw std::runtime_error(msg);
    }
}

namespace jxx::nio
{
    MappedByteBuffer::MappedByteBuffer(std::shared_ptr<std::vector<jxx::lang::jbyte>> storage,
                                       jxx::lang::jint offset,
                                       jxx::lang::jint capacity,
                                       jxx::lang::jbool readOnly,
                                       const std::string& path,
                                       jxx::lang::jlong fileOffset,
                                       jxx::Ptr<ByteOrder> order)
        : ByteBuffer(std::move(storage), offset, capacity, readOnly, true, std::move(order)),
          path_(path),
          fileOffset_(fileOffset)
    {
    }

    jxx::Ptr<MappedByteBuffer> MappedByteBuffer::mapFile(
        const std::string& path,
        jxx::lang::jlong fileOffset,
        jxx::lang::jint size,
        jxx::lang::jbool readOnly,
        jxx::Ptr<ByteOrder> order)
    {
        if (fileOffset < 0 || size < 0)
            throwIAE_("negative map offset/size");

        auto storage = std::make_shared<std::vector<jxx::lang::jbyte>>(static_cast<std::size_t>(size));
        std::ifstream in(path, std::ios::binary);
        if (in.good())
        {
            in.seekg(static_cast<std::streamoff>(fileOffset), std::ios::beg);
            in.read(reinterpret_cast<char*>(storage->data()), static_cast<std::streamsize>(size));
        }
        else if (readOnly)
        {
            throwIOE_("failed to open mapped file for reading");
        }

        auto out = std::shared_ptr<MappedByteBuffer>(new MappedByteBuffer(storage, 0, size, readOnly, path, fileOffset, std::move(order)));
        out->position(0);
        out->limit(size);
        return out;
    }

    jxx::Ptr<MappedByteBuffer> MappedByteBuffer::self_Mapped_()
    {
        if (thisPtr)
            return std::static_pointer_cast<MappedByteBuffer>(thisPtr);
        return std::static_pointer_cast<MappedByteBuffer>(shared_from_this());
    }

    jxx::lang::jbool MappedByteBuffer::isLoaded() const { return loaded_; }
    jxx::Ptr<MappedByteBuffer> MappedByteBuffer::load() { loaded_ = true; return self_Mapped_(); }

    jxx::Ptr<MappedByteBuffer> MappedByteBuffer::force()
    {
        if (!isReadOnly())
        {
            std::fstream io(path_, std::ios::binary | std::ios::in | std::ios::out);
            if (!io.good())
                io.open(path_, std::ios::binary | std::ios::out);
            if (!io.good())
                throwIOE_("failed to open mapped file for writing");

            io.seekp(static_cast<std::streamoff>(fileOffset_), std::ios::beg);
            io.write(reinterpret_cast<const char*>(storage_->data()) + offset_, static_cast<std::streamsize>(capacity()));
            io.flush();
        }
        return self_Mapped_();
    }

    jxx::Ptr<MappedByteBuffer> MappedByteBuffer::sliceMapped() const
    {
        auto out = std::shared_ptr<MappedByteBuffer>(new MappedByteBuffer(storage_, offset_ + pos_, remaining(), isReadOnly(), path_, fileOffset_ + pos_, order_));
        out->position(0);
        out->limit(remaining());
        return out;
    }

    jxx::Ptr<MappedByteBuffer> MappedByteBuffer::duplicateMapped() const
    {
        auto out = std::shared_ptr<MappedByteBuffer>(new MappedByteBuffer(storage_, offset_, capacity(), isReadOnly(), path_, fileOffset_, order_));
        out->mark_ = mark_;
        out->pos_ = pos_;
        out->lim_ = lim_;
        return out;
    }

    jxx::Ptr<MappedByteBuffer> MappedByteBuffer::asReadOnlyMappedBuffer() const
    {
        auto out = std::shared_ptr<MappedByteBuffer>(new MappedByteBuffer(storage_, offset_, capacity(), true, path_, fileOffset_, order_));
        out->mark_ = mark_;
        out->pos_ = pos_;
        out->lim_ = lim_;
        return out;
    }
}
