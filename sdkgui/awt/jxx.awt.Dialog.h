#pragma once

#include "awt/jxx.awt.Window.h"
#include "lang/jxx.lang.String.h"

namespace jxx::awt
{
    class Frame;
    class GraphicsConfiguration;

    class Dialog : public ::jxx::lang::ClassBase<Dialog, Window>
    {
    public:
        using JxxSuper = Window;
        using Super = ::jxx::lang::ClassBase<Dialog, JxxSuper>;

        static constexpr ::jxx::lang::jint MODELESS = 0;
        static constexpr ::jxx::lang::jint DOCUMENT_MODAL = 1;
        static constexpr ::jxx::lang::jint APPLICATION_MODAL = 2;
        static constexpr ::jxx::lang::jint TOOLKIT_MODAL = 3;
        static constexpr ::jxx::lang::jint DEFAULT_MODALITY_TYPE =
            APPLICATION_MODAL;

        explicit Dialog(const ::jxx::Ptr<Frame>& owner);
        Dialog(const ::jxx::Ptr<Frame>& owner,
            const ::jxx::Ptr<::jxx::lang::String>& title);
        Dialog(const ::jxx::Ptr<Frame>& owner,
            const ::jxx::Ptr<::jxx::lang::String>& title,
            ::jxx::lang::jbool modal);
        Dialog(const ::jxx::Ptr<Frame>& owner,
            const ::jxx::Ptr<::jxx::lang::String>& title,
            ::jxx::lang::jbool modal,
            const ::jxx::Ptr<GraphicsConfiguration>& configuration);
        explicit Dialog(const ::jxx::Ptr<Dialog>& owner);
        Dialog(const ::jxx::Ptr<Dialog>& owner,
            const ::jxx::Ptr<::jxx::lang::String>& title);
        Dialog(const ::jxx::Ptr<Dialog>& owner,
            const ::jxx::Ptr<::jxx::lang::String>& title,
            ::jxx::lang::jbool modal);
        Dialog(const ::jxx::Ptr<Dialog>& owner,
            const ::jxx::Ptr<::jxx::lang::String>& title,
            ::jxx::lang::jbool modal,
            const ::jxx::Ptr<GraphicsConfiguration>& configuration);
        ~Dialog() override = default;

        virtual ::jxx::Ptr<::jxx::lang::String> getTitle() const;
        virtual void setTitle(
            const ::jxx::Ptr<::jxx::lang::String>& title);
        virtual ::jxx::lang::jbool isModal() const;
        virtual void setModal(::jxx::lang::jbool modal);
        virtual ::jxx::lang::jint getModalityType() const;
        virtual void setModalityType(::jxx::lang::jint modalityType);
        virtual ::jxx::lang::jbool isResizable() const;
        virtual void setResizable(::jxx::lang::jbool resizable);
        virtual ::jxx::lang::jbool isUndecorated() const;
        virtual void setUndecorated(::jxx::lang::jbool undecorated);
        virtual ::jxx::Ptr<Window> getOwner() const;
        void validate() override;

    protected:
        void ensureNativeWindow() override;
        ::jxx::Ptr<::jxx::lang::String> paramString() const;

    private:
        Dialog(const ::jxx::Ptr<Window>& owner,
            const ::jxx::Ptr<::jxx::lang::String>& title,
            ::jxx::lang::jbool modal,
            const ::jxx::Ptr<GraphicsConfiguration>& configuration,
            ::jxx::lang::jbool internalTag);

        std::weak_ptr<Window> owner_;
        ::jxx::Ptr<::jxx::lang::String> title_;
        ::jxx::Ptr<GraphicsConfiguration> graphicsConfiguration_;
        ::jxx::lang::jint modalityType_;
        ::jxx::lang::jbool resizable_;
        ::jxx::lang::jbool undecorated_;
    };
}
