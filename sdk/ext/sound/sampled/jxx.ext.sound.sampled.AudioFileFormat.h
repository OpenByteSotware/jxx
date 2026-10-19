#pragma once

#include "ext/sound/sampled/jxx.ext.sound.sampled.AudioFormat.h"
#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.String.h"
#include "lang/jxx_types.h"

namespace jxx::ext::sound::sampled
{
    class AudioFileFormat final
        : public ::jxx::lang::ClassBase<AudioFileFormat, ::jxx::lang::Object>
    {
    public:
        using JxxSuper = ::jxx::lang::Object;
        using Super = ::jxx::lang::ClassBase<AudioFileFormat, JxxSuper>;

        class Type final
            : public ::jxx::lang::ClassBase<Type, ::jxx::lang::Object>
        {
        public:
            using JxxSuper = ::jxx::lang::Object;
            using Super = ::jxx::lang::ClassBase<Type, JxxSuper>;

            Type(const ::jxx::Ptr<::jxx::lang::String>& name,
                 const ::jxx::Ptr<::jxx::lang::String>& extension);
            auto getExtension() const -> ::jxx::Ptr<::jxx::lang::String>;
            auto toString() const -> ::jxx::Ptr<::jxx::lang::String> override;

            static ::jxx::Ptr<Type> WAVE;
            static ::jxx::Ptr<Type> AU;
            static ::jxx::Ptr<Type> AIFF;
            static ::jxx::Ptr<Type> AIFC;
            static ::jxx::Ptr<Type> SND;

        private:
            ::jxx::Ptr<::jxx::lang::String> name_;
            ::jxx::Ptr<::jxx::lang::String> extension_;
        };

        AudioFileFormat(const ::jxx::Ptr<Type>& type,
                        ::jxx::lang::jint byteLength,
                        const ::jxx::Ptr<AudioFormat>& format,
                        ::jxx::lang::jint frameLength);

        auto getType() const -> ::jxx::Ptr<Type>;
        auto getByteLength() const -> ::jxx::lang::jint;
        auto getFormat() const -> ::jxx::Ptr<AudioFormat>;
        auto getFrameLength() const -> ::jxx::lang::jint;
        auto toString() const -> ::jxx::Ptr<::jxx::lang::String> override;

    private:
        ::jxx::Ptr<Type> type_;
        ::jxx::lang::jint byteLength_;
        ::jxx::Ptr<AudioFormat> format_;
        ::jxx::lang::jint frameLength_;
    };
}
