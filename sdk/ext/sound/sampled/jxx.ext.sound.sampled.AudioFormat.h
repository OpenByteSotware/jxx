#pragma once

#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.String.h"
#include "lang/jxx_types.h"

namespace jxx::ext::sound::sampled
{
    class AudioFormat final
        : public ::jxx::lang::ClassBase<AudioFormat, ::jxx::lang::Object>
    {
    public:
        using JxxSuper = ::jxx::lang::Object;
        using Super = ::jxx::lang::ClassBase<AudioFormat, JxxSuper>;

        class Encoding final
            : public ::jxx::lang::ClassBase<Encoding, ::jxx::lang::Object>
        {
        public:
            using JxxSuper = ::jxx::lang::Object;
            using Super = ::jxx::lang::ClassBase<Encoding, JxxSuper>;

            explicit Encoding(const ::jxx::Ptr<::jxx::lang::String>& name);
            auto toString() const -> ::jxx::Ptr<::jxx::lang::String> override;

            static ::jxx::Ptr<Encoding> PCM_SIGNED;
            static ::jxx::Ptr<Encoding> PCM_UNSIGNED;
            static ::jxx::Ptr<Encoding> PCM_FLOAT;
            static ::jxx::Ptr<Encoding> ULAW;
            static ::jxx::Ptr<Encoding> ALAW;

        private:
            ::jxx::Ptr<::jxx::lang::String> name_;
        };

        AudioFormat(
            const ::jxx::Ptr<Encoding>& encoding,
            ::jxx::lang::jfloat sampleRate,
            ::jxx::lang::jint sampleSizeInBits,
            ::jxx::lang::jint channels,
            ::jxx::lang::jint frameSize,
            ::jxx::lang::jfloat frameRate,
            ::jxx::lang::jbool bigEndian);

        auto getEncoding() const -> ::jxx::Ptr<Encoding>;
        auto getSampleRate() const -> ::jxx::lang::jfloat;
        auto getSampleSizeInBits() const -> ::jxx::lang::jint;
        auto getChannels() const -> ::jxx::lang::jint;
        auto getFrameSize() const -> ::jxx::lang::jint;
        auto getFrameRate() const -> ::jxx::lang::jfloat;
        auto isBigEndian() const -> ::jxx::lang::jbool;
        auto toString() const -> ::jxx::Ptr<::jxx::lang::String> override;

    private:
        ::jxx::Ptr<Encoding> encoding_;
        ::jxx::lang::jfloat sampleRate_;
        ::jxx::lang::jint sampleSizeInBits_;
        ::jxx::lang::jint channels_;
        ::jxx::lang::jint frameSize_;
        ::jxx::lang::jfloat frameRate_;
        ::jxx::lang::jbool bigEndian_;
    };
}
