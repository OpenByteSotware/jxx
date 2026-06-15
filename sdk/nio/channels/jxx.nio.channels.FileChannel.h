#pragma once

#include <fstream>
#include <string>

#include "lang/jxx_types.h"
#include "lang/jxx.lang.String.h"
#include "io/jxx.io.File.h"
#include "nio/jxx.nio.ByteBuffer.h"
#include "nio/jxx.nio.MappedByteBuffer.h"
#include "nio/channels/jxx.nio.channels.SeekableByteChannel.h"

namespace jxx::nio::channels
{
    class FileChannel final : public SeekableByteChannel
    {
    public:
        class MapMode final : public jxx::lang::Object
        {
        public:
            static jxx::Ptr<MapMode> READ_ONLY;
            static jxx::Ptr<MapMode> READ_WRITE;
            static jxx::Ptr<MapMode> PRIVATE_;

            explicit MapMode(jxx::Ptr<jxx::lang::String> name);
            ~MapMode() override = default;

            jxx::Ptr<jxx::lang::String> toString() const override;
            jxx::lang::jbool equals(jxx::Ptr<jxx::lang::Object> other) const override;
            jxx::lang::jint hashCode() const override;

        private:
            jxx::Ptr<jxx::lang::String> name_;
        };

    public:
        static jxx::Ptr<FileChannel> open(jxx::Ptr<jxx::io::File> file,
                                          jxx::lang::jbool readable,
                                          jxx::lang::jbool writable,
                                          jxx::lang::jbool append = false);

        ~FileChannel() override;

    public:
        jxx::lang::jbool isOpen() const override;
        void close() override;

        jxx::lang::jint read(jxx::Ptr<jxx::nio::ByteBuffer> dst) override;
        jxx::lang::jint write(jxx::Ptr<jxx::nio::ByteBuffer> src) override;

        jxx::lang::jlong position() const override;
        jxx::Ptr<SeekableByteChannel> position(jxx::lang::jlong newPosition) override;
        jxx::lang::jlong size() const override;
        jxx::Ptr<SeekableByteChannel> truncate(jxx::lang::jlong size) override;

        void force(jxx::lang::jbool metaData);
        jxx::Ptr<jxx::nio::MappedByteBuffer> map(jxx::Ptr<MapMode> mode,
                                                 jxx::lang::jlong position,
                                                 jxx::lang::jlong size);

        jxx::Ptr<jxx::io::File> file() const;

    private:
        FileChannel(jxx::Ptr<jxx::io::File> file,
                    jxx::lang::jbool readable,
                    jxx::lang::jbool writable,
                    jxx::lang::jbool append);

        void ensureOpen_() const;
        jxx::Ptr<FileChannel> self_FileChannel_();

    private:
        jxx::Ptr<jxx::io::File> file_;
        mutable std::fstream stream_;
        jxx::lang::jbool readable_ = false;
        jxx::lang::jbool writable_ = false;
        jxx::lang::jbool append_ = false;
        jxx::lang::jbool open_ = false;
    };
}
