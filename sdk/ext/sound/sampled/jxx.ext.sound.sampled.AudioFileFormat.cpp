#include "ext/sound/sampled/jxx.ext.sound.sampled.AudioFileFormat.h"

#include <sstream>

namespace jxx::ext::sound::sampled
{
    ::jxx::Ptr<AudioFileFormat::Type> AudioFileFormat::Type::WAVE =
        ::jxx::NEW<Type>(::jxx::NEW<::jxx::lang::String>("WAVE"), ::jxx::NEW<::jxx::lang::String>("wav"));
    ::jxx::Ptr<AudioFileFormat::Type> AudioFileFormat::Type::AU =
        ::jxx::NEW<Type>(::jxx::NEW<::jxx::lang::String>("AU"), ::jxx::NEW<::jxx::lang::String>("au"));
    ::jxx::Ptr<AudioFileFormat::Type> AudioFileFormat::Type::AIFF =
        ::jxx::NEW<Type>(::jxx::NEW<::jxx::lang::String>("AIFF"), ::jxx::NEW<::jxx::lang::String>("aif"));
    ::jxx::Ptr<AudioFileFormat::Type> AudioFileFormat::Type::AIFC =
        ::jxx::NEW<Type>(::jxx::NEW<::jxx::lang::String>("AIFF-C"), ::jxx::NEW<::jxx::lang::String>("aifc"));
    ::jxx::Ptr<AudioFileFormat::Type> AudioFileFormat::Type::SND =
        ::jxx::NEW<Type>(::jxx::NEW<::jxx::lang::String>("SND"), ::jxx::NEW<::jxx::lang::String>("snd"));

    AudioFileFormat::Type::Type(
        const ::jxx::Ptr<::jxx::lang::String>& name,
        const ::jxx::Ptr<::jxx::lang::String>& extension)
        : Super(), name_(name), extension_(extension)
    {
    }

    auto AudioFileFormat::Type::getExtension() const
        -> ::jxx::Ptr<::jxx::lang::String> { return extension_; }

    auto AudioFileFormat::Type::toString() const
        -> ::jxx::Ptr<::jxx::lang::String> { return name_; }

    AudioFileFormat::AudioFileFormat(
        const ::jxx::Ptr<Type>& type,
        ::jxx::lang::jint byteLength,
        const ::jxx::Ptr<AudioFormat>& format,
        ::jxx::lang::jint frameLength)
        : Super(), type_(type), byteLength_(byteLength),
          format_(format), frameLength_(frameLength)
    {
    }

    auto AudioFileFormat::getType() const -> ::jxx::Ptr<Type> { return type_; }
    auto AudioFileFormat::getByteLength() const -> ::jxx::lang::jint { return byteLength_; }
    auto AudioFileFormat::getFormat() const -> ::jxx::Ptr<AudioFormat> { return format_; }
    auto AudioFileFormat::getFrameLength() const -> ::jxx::lang::jint { return frameLength_; }

    auto AudioFileFormat::toString() const -> ::jxx::Ptr<::jxx::lang::String>
    {
        std::ostringstream text;
        text << type_->toString()->utf8() << " (." << type_->getExtension()->utf8()
             << ") file, byte length: " << byteLength_
             << ", data format: " << format_->toString()->utf8()
             << ", frame length: " << frameLength_;
        return ::jxx::NEW<::jxx::lang::String>(text.str());
    }
}
