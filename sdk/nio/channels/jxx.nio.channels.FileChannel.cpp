#include "nio/channels/jxx.nio.channels.FileChannel.h"

#include <filesystem>
#include <stdexcept>

namespace fs = std::filesystem;

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

namespace jxx::nio::channels
{
    jxx::Ptr<FileChannel::MapMode> FileChannel::MapMode::READ_ONLY = std::make_shared<FileChannel::MapMode>(std::make_shared<jxx::lang::String>("READ_ONLY"));
    jxx::Ptr<FileChannel::MapMode> FileChannel::MapMode::READ_WRITE = std::make_shared<FileChannel::MapMode>(std::make_shared<jxx::lang::String>("READ_WRITE"));
    jxx::Ptr<FileChannel::MapMode> FileChannel::MapMode::PRIVATE_ = std::make_shared<FileChannel::MapMode>(std::make_shared<jxx::lang::String>("PRIVATE"));

    FileChannel::MapMode::MapMode(jxx::Ptr<jxx::lang::String> name) : name_(std::move(name)) {}
    jxx::Ptr<jxx::lang::String> FileChannel::MapMode::toString() const { return name_; }
    jxx::lang::jbool FileChannel::MapMode::equals(jxx::Ptr<jxx::lang::Object> other) const
    {
        auto o = std::dynamic_pointer_cast<MapMode>(other);
        return o && name_ && o->name_ && name_->equals(o->name_);
    }
    jxx::lang::jint FileChannel::MapMode::hashCode() const { return name_ ? name_->hashCode() : 0; }

    jxx::Ptr<FileChannel> FileChannel::open(jxx::Ptr<jxx::io::File> file,
                                            jxx::lang::jbool readable,
                                            jxx::lang::jbool writable,
                                            jxx::lang::jbool append)
    {
        return std::shared_ptr<FileChannel>(new FileChannel(std::move(file), readable, writable, append));
    }

    FileChannel::FileChannel(jxx::Ptr<jxx::io::File> file,
                             jxx::lang::jbool readable,
                             jxx::lang::jbool writable,
                             jxx::lang::jbool append)
        : file_(std::move(file)), readable_(readable), writable_(writable), append_(append)
    {
        if (!file_)
            throwIAE_("null file");
        if (!readable_ && !writable_)
            throwIAE_("channel must be readable or writable");

        std::ios::openmode mode = std::ios::binary;
        if (readable_)
            mode |= std::ios::in;
        if (writable_)
            mode |= std::ios::out;
        if (append_)
            mode |= std::ios::app;
        if (writable_ && !append_)
            mode |= std::ios::in;

        stream_.open(file_->getPath()->utf8(), mode);
        if (!stream_.good() && writable_)
        {
            std::ofstream create(file_->getPath()->utf8(), std::ios::binary | std::ios::app);
            create.close();
            stream_.open(file_->getPath()->utf8(), mode);
        }
        if (!stream_.good())
            throwIOE_("failed to open file channel");
        open_ = true;
    }

    FileChannel::~FileChannel() { try { close(); } catch (...) {} }

    void FileChannel::ensureOpen_() const
    {
        if (!open_)
            throwIOE_("channel closed");
    }

    jxx::Ptr<FileChannel> FileChannel::self_FileChannel_()
    {
        if (thisPtr)
            return std::static_pointer_cast<FileChannel>(thisPtr);
        return std::static_pointer_cast<FileChannel>(shared_from_this());
    }

    jxx::lang::jbool FileChannel::isOpen() const { return open_; }

    void FileChannel::close()
    {
        if (open_)
        {
            stream_.flush();
            stream_.close();
            open_ = false;
        }
    }

    jxx::lang::jint FileChannel::read(jxx::Ptr<jxx::nio::ByteBuffer> dst)
    {
        ensureOpen_();
        if (!readable_)
            throwIOE_("channel not readable");
        if (!dst)
            throwIAE_("null destination buffer");
        if (!dst->hasRemaining())
            return 0;

        auto temp = jxx::NEW<jxx::lang::ByteArrayType>(dst->remaining());
        stream_.read(reinterpret_cast<char*>(&(*temp)[(jxx::lang::jint)0]), static_cast<std::streamsize>(temp->length));
        const auto n = static_cast<jxx::lang::jint>(stream_.gcount());
        if (n <= 0)
            return -1;
        dst->put(temp, 0, n);
        return n;
    }

    jxx::lang::jint FileChannel::write(jxx::Ptr<jxx::nio::ByteBuffer> src)
    {
        ensureOpen_();
        if (!writable_)
            throwIOE_("channel not writable");
        if (!src)
            throwIAE_("null source buffer");
        const auto n = src->remaining();
        if (n <= 0)
            return 0;

        auto temp = jxx::NEW<jxx::lang::ByteArrayType>(n);
        src->get(temp, 0, n);
        stream_.write(reinterpret_cast<const char*>(&(*temp)[0]), static_cast<std::streamsize>(n));
        if (!stream_.good())
            throwIOE_("write failed");
        return n;
    }

    jxx::lang::jlong FileChannel::position() const
    {
        ensureOpen_();
        auto g = stream_.tellg();
        if (g >= 0)
            return static_cast<jxx::lang::jlong>(g);
        auto p = stream_.tellp();
        return p >= 0 ? static_cast<jxx::lang::jlong>(p) : 0;
    }

    jxx::Ptr<SeekableByteChannel> FileChannel::position(jxx::lang::jlong newPosition)
    {
        ensureOpen_();
        if (newPosition < 0)
            throwIAE_("negative position");
        stream_.seekg(static_cast<std::streamoff>(newPosition), std::ios::beg);
        stream_.seekp(static_cast<std::streamoff>(newPosition), std::ios::beg);
        return std::static_pointer_cast<SeekableByteChannel>(self_FileChannel_());
    }

    jxx::lang::jlong FileChannel::size() const
    {
        ensureOpen_();
        std::error_code ec;
        const auto sz = fs::file_size(file_->getPath()->utf8(), ec);
        return ec ? 0 : static_cast<jxx::lang::jlong>(sz);
    }

    jxx::Ptr<SeekableByteChannel> FileChannel::truncate(jxx::lang::jlong size)
    {
        ensureOpen_();
        if (!writable_)
            throwIOE_("channel not writable");
        if (size < 0)
            throwIAE_("negative size");
        stream_.flush();
        std::error_code ec;
        fs::resize_file(file_->getPath()->utf8(), static_cast<std::uintmax_t>(size), ec);
        if (ec)
            throwIOE_("truncate failed");
        if (position() > size)
            position(size);
        return std::static_pointer_cast<SeekableByteChannel>(self_FileChannel_());
    }

    void FileChannel::force(jxx::lang::jbool /*metaData*/)
    {
        ensureOpen_();
        stream_.flush();
    }

    jxx::Ptr<jxx::nio::MappedByteBuffer> FileChannel::map(jxx::Ptr<MapMode> mode,
                                                          jxx::lang::jlong position,
                                                          jxx::lang::jlong size)
    {
        ensureOpen_();
        if (!mode)
            throwIAE_("null map mode");
        if (position < 0 || size < 0)
            throwIAE_("negative map position/size");
        const auto ro = mode->equals(MapMode::READ_ONLY);
        return jxx::nio::MappedByteBuffer::mapFile(file_->getPath()->utf8(), position, static_cast<jxx::lang::jint>(size), ro, jxx::nio::ByteOrder::BIG_ENDIAN);
    }

    jxx::Ptr<jxx::io::File> FileChannel::file() const { return file_; }
}
