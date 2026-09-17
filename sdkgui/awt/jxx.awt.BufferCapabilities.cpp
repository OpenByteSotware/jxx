#include "awt/jxx.awt.BufferCapabilities.h"

#include "lang/jxx.lang.IllegalArgumentException.h"

namespace jxx::awt
{
    BufferCapabilities::FlipContents::FlipContents(const char* name)
        : name_(::jxx::NEW<::jxx::lang::String>(name))
    {
    }

    ::jxx::Ptr<::jxx::lang::String>
    BufferCapabilities::FlipContents::toString() const
    {
        return name_;
    }

    ::jxx::Ptr<BufferCapabilities::FlipContents>
        BufferCapabilities::FlipContents::UNDEFINED(
            new BufferCapabilities::FlipContents("undefined"));
    ::jxx::Ptr<BufferCapabilities::FlipContents>
        BufferCapabilities::FlipContents::BACKGROUND(
            new BufferCapabilities::FlipContents("background"));
    ::jxx::Ptr<BufferCapabilities::FlipContents>
        BufferCapabilities::FlipContents::PRIOR(
            new BufferCapabilities::FlipContents("prior"));
    ::jxx::Ptr<BufferCapabilities::FlipContents>
        BufferCapabilities::FlipContents::COPIED(
            new BufferCapabilities::FlipContents("copied"));

    BufferCapabilities::BufferCapabilities(
        const ::jxx::Ptr<ImageCapabilities>& frontCapabilities,
        const ::jxx::Ptr<ImageCapabilities>& backCapabilities,
        const ::jxx::Ptr<FlipContents>& flipContents)
        : frontCapabilities_(frontCapabilities),
          backCapabilities_(backCapabilities), flipContents_(flipContents)
    {
        if (frontCapabilities_ == nullptr || backCapabilities_ == nullptr)
            throw ::jxx::lang::IllegalArgumentException("capabilities");
    }

    ::jxx::Ptr<ImageCapabilities>
    BufferCapabilities::getFrontBufferCapabilities() const
    {
        return frontCapabilities_;
    }

    ::jxx::Ptr<ImageCapabilities>
    BufferCapabilities::getBackBufferCapabilities() const
    {
        return backCapabilities_;
    }

    ::jxx::lang::jbool BufferCapabilities::isPageFlipping() const
    {
        return flipContents_ != nullptr;
    }

    ::jxx::Ptr<BufferCapabilities::FlipContents>
    BufferCapabilities::getFlipContents() const
    {
        return flipContents_;
    }

    ::jxx::lang::jbool BufferCapabilities::isFullScreenRequired() const
    {
        return false;
    }

    ::jxx::lang::jbool BufferCapabilities::isMultiBufferAvailable() const
    {
        return false;
    }

    ::jxx::Ptr<::jxx::lang::Object> BufferCapabilities::clone() const
    {
        return ::jxx::NEW<BufferCapabilities>(frontCapabilities_,
            backCapabilities_, flipContents_);
    }
}
