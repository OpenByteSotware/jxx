#pragma once

#include "awt/jxx.awt.ImageCapabilities.h"
#include "lang/jxx.lang.String.h"

namespace jxx::awt
{
    class BufferCapabilities :
        public ::jxx::lang::ClassBase<BufferCapabilities, ::jxx::lang::Object>
    {
    public:
        class FlipContents :
            public ::jxx::lang::ClassBase<FlipContents, ::jxx::lang::Object>
        {
        public:
            using JxxSuper = ::jxx::lang::Object;
            using Super = ::jxx::lang::ClassBase<FlipContents, JxxSuper>;

            static ::jxx::Ptr<FlipContents> UNDEFINED;
            static ::jxx::Ptr<FlipContents> BACKGROUND;
            static ::jxx::Ptr<FlipContents> PRIOR;
            static ::jxx::Ptr<FlipContents> COPIED;

            ::jxx::Ptr<::jxx::lang::String> toString() const override;

        private:
            friend class BufferCapabilities;
            explicit FlipContents(const char* name);
            ::jxx::Ptr<::jxx::lang::String> name_;
        };

        using JxxSuper = ::jxx::lang::Object;
        using Super = ::jxx::lang::ClassBase<BufferCapabilities, JxxSuper>;

        BufferCapabilities(
            const ::jxx::Ptr<ImageCapabilities>& frontCapabilities,
            const ::jxx::Ptr<ImageCapabilities>& backCapabilities,
            const ::jxx::Ptr<FlipContents>& flipContents);
        ~BufferCapabilities() override = default;

        ::jxx::Ptr<ImageCapabilities> getFrontBufferCapabilities() const;
        ::jxx::Ptr<ImageCapabilities> getBackBufferCapabilities() const;
        ::jxx::lang::jbool isPageFlipping() const;
        ::jxx::Ptr<FlipContents> getFlipContents() const;
        virtual ::jxx::lang::jbool isFullScreenRequired() const;
        virtual ::jxx::lang::jbool isMultiBufferAvailable() const;
        ::jxx::Ptr<::jxx::lang::Object> clone() const override;

    private:
        ::jxx::Ptr<ImageCapabilities> frontCapabilities_;
        ::jxx::Ptr<ImageCapabilities> backCapabilities_;
        ::jxx::Ptr<FlipContents> flipContents_;
    };
}
