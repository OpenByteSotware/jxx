#include "ext/sound/sampled/jxx.ext.sound.sampled.AudioFormat.h"

#include <sstream>

namespace jxx::ext::sound::sampled
{
    ::jxx::Ptr<AudioFormat::Encoding> AudioFormat::Encoding::PCM_SIGNED =
        ::jxx::NEW<AudioFormat::Encoding>(::jxx::NEW<::jxx::lang::String>("PCM_SIGNED"));
    ::jxx::Ptr<AudioFormat::Encoding> AudioFormat::Encoding::PCM_UNSIGNED =
        ::jxx::NEW<AudioFormat::Encoding>(::jxx::NEW<::jxx::lang::String>("PCM_UNSIGNED"));
    ::jxx::Ptr<AudioFormat::Encoding> AudioFormat::Encoding::PCM_FLOAT =
        ::jxx::NEW<AudioFormat::Encoding>(::jxx::NEW<::jxx::lang::String>("PCM_FLOAT"));
    ::jxx::Ptr<AudioFormat::Encoding> AudioFormat::Encoding::ULAW =
        ::jxx::NEW<AudioFormat::Encoding>(::jxx::NEW<::jxx::lang::String>("ULAW"));
    ::jxx::Ptr<AudioFormat::Encoding> AudioFormat::Encoding::ALAW =
        ::jxx::NEW<AudioFormat::Encoding>(::jxx::NEW<::jxx::lang::String>("ALAW"));

    AudioFormat::Encoding::Encoding(const ::jxx::Ptr<::jxx::lang::String>& name)
        : Super(), name_(name)
    {
    }

    auto AudioFormat::Encoding::toString() const
        -> ::jxx::Ptr<::jxx::lang::String>
    {
        return name_;
    }

    AudioFormat::AudioFormat(
        const ::jxx::Ptr<Encoding>& encoding,
        ::jxx::lang::jfloat sampleRate,
        ::jxx::lang::jint sampleSizeInBits,
        ::jxx::lang::jint channels,
        ::jxx::lang::jint frameSize,
        ::jxx::lang::jfloat frameRate,
        ::jxx::lang::jbool bigEndian)
        : Super(), encoding_(encoding), sampleRate_(sampleRate),
          sampleSizeInBits_(sampleSizeInBits), channels_(channels),
          frameSize_(frameSize), frameRate_(frameRate), bigEndian_(bigEndian)
    {
    }

    auto AudioFormat::getEncoding() const -> ::jxx::Ptr<Encoding> { return encoding_; }
    auto AudioFormat::getSampleRate() const -> ::jxx::lang::jfloat { return sampleRate_; }
    auto AudioFormat::getSampleSizeInBits() const -> ::jxx::lang::jint { return sampleSizeInBits_; }
    auto AudioFormat::getChannels() const -> ::jxx::lang::jint { return channels_; }
    auto AudioFormat::getFrameSize() const -> ::jxx::lang::jint { return frameSize_; }
    auto AudioFormat::getFrameRate() const -> ::jxx::lang::jfloat { return frameRate_; }
    auto AudioFormat::isBigEndian() const -> ::jxx::lang::jbool { return bigEndian_; }

    auto AudioFormat::toString() const -> ::jxx::Ptr<::jxx::lang::String>
    {
        std::ostringstream text;
        text << encoding_->toString()->utf8() << " " << sampleRate_
             << " Hz, " << sampleSizeInBits_ << " bit, " << channels_
             << " channels, " << frameSize_ << " bytes/frame";
        return ::jxx::NEW<::jxx::lang::String>(text.str());
    }
}
