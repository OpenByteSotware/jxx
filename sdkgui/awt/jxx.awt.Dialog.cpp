#include "awt/jxx.awt.Dialog.h"

#include <sstream>

#include "awt/jxx.awt.BorderLayout.h"
#include "awt/jxx.awt.Frame.h"
#include "awt/jxx.awt.IllegalComponentStateException.h"
#include "gui/internal/jxx.gui.internal.NativeWindow.h"
#include "gui/internal/jxx.gui.internal.WxFramePeer.h"
#include "lang/jxx.lang.IllegalArgumentException.h"

namespace jxx::awt
{
    namespace
    {
        ::jxx::Ptr<::jxx::lang::String> emptyTitle()
        {
            return ::jxx::NEW<::jxx::lang::String>("");
        }
    }

    Dialog::Dialog(const ::jxx::Ptr<Frame>& owner)
        : Dialog(::jxx::CAST<Window>(owner), emptyTitle(), false,
              nullptr, true)
    {
    }

    Dialog::Dialog(const ::jxx::Ptr<Frame>& owner,
        const ::jxx::Ptr<::jxx::lang::String>& title)
        : Dialog(::jxx::CAST<Window>(owner), title, false, nullptr, true)
    {
    }

    Dialog::Dialog(const ::jxx::Ptr<Frame>& owner,
        const ::jxx::Ptr<::jxx::lang::String>& title,
        ::jxx::lang::jbool modal)
        : Dialog(::jxx::CAST<Window>(owner), title, modal, nullptr, true)
    {
    }

    Dialog::Dialog(const ::jxx::Ptr<Frame>& owner,
        const ::jxx::Ptr<::jxx::lang::String>& title,
        ::jxx::lang::jbool modal,
        const ::jxx::Ptr<GraphicsConfiguration>& configuration)
        : Dialog(::jxx::CAST<Window>(owner), title, modal,
              configuration, true)
    {
    }

    Dialog::Dialog(const ::jxx::Ptr<Dialog>& owner)
        : Dialog(::jxx::CAST<Window>(owner), emptyTitle(), false,
              nullptr, true)
    {
    }

    Dialog::Dialog(const ::jxx::Ptr<Dialog>& owner,
        const ::jxx::Ptr<::jxx::lang::String>& title)
        : Dialog(::jxx::CAST<Window>(owner), title, false, nullptr, true)
    {
    }

    Dialog::Dialog(const ::jxx::Ptr<Dialog>& owner,
        const ::jxx::Ptr<::jxx::lang::String>& title,
        ::jxx::lang::jbool modal)
        : Dialog(::jxx::CAST<Window>(owner), title, modal, nullptr, true)
    {
    }

    Dialog::Dialog(const ::jxx::Ptr<Dialog>& owner,
        const ::jxx::Ptr<::jxx::lang::String>& title,
        ::jxx::lang::jbool modal,
        const ::jxx::Ptr<GraphicsConfiguration>& configuration)
        : Dialog(::jxx::CAST<Window>(owner), title, modal,
              configuration, true)
    {
    }

    Dialog::Dialog(const ::jxx::Ptr<Window>& owner,
        const ::jxx::Ptr<::jxx::lang::String>& title,
        ::jxx::lang::jbool modal,
        const ::jxx::Ptr<GraphicsConfiguration>& configuration,
        ::jxx::lang::jbool)
        : owner_(owner),
          title_(title != nullptr ? title : emptyTitle()),
          graphicsConfiguration_(configuration),
          modalityType_(modal ? DEFAULT_MODALITY_TYPE : MODELESS),
          resizable_(true),
          undecorated_(false)
    {
        setLayout(::jxx::NEW<BorderLayout>());
    }

    ::jxx::Ptr<::jxx::lang::String> Dialog::getTitle() const
    {
        return title_;
    }

    void Dialog::setTitle(
        const ::jxx::Ptr<::jxx::lang::String>& title)
    {
        title_ = title != nullptr ? title : emptyTitle();
        if (nativeWindow_ != nullptr) nativeWindow_->setTitle(title_);
    }

    ::jxx::lang::jbool Dialog::isModal() const
    {
        return modalityType_ != MODELESS;
    }

    void Dialog::setModal(::jxx::lang::jbool modal)
    {
        setModalityType(modal ? DEFAULT_MODALITY_TYPE : MODELESS);
    }

    ::jxx::lang::jint Dialog::getModalityType() const
    {
        return modalityType_;
    }

    void Dialog::setModalityType(::jxx::lang::jint modalityType)
    {
        if (modalityType < MODELESS || modalityType > TOOLKIT_MODAL)
            throw ::jxx::lang::IllegalArgumentException("modalityType");
        modalityType_ = modalityType;
    }

    ::jxx::lang::jbool Dialog::isResizable() const
    {
        return resizable_;
    }

    void Dialog::setResizable(::jxx::lang::jbool resizable)
    {
        resizable_ = resizable;
    }

    ::jxx::lang::jbool Dialog::isUndecorated() const
    {
        return undecorated_;
    }

    void Dialog::setUndecorated(::jxx::lang::jbool undecorated)
    {
        if (isDisplayable())
            throw IllegalComponentStateException(
                ::jxx::NEW<::jxx::lang::String>(
                    "dialog is already displayable"));
        undecorated_ = undecorated;
    }

    ::jxx::Ptr<Window> Dialog::getOwner() const
    {
        return owner_.lock();
    }

    void Dialog::ensureNativeWindow()
    {
        if (nativeWindow_ != nullptr) return;
        nativeWindow_ = ::jxx::NEW<::jxx::gui::internal::WxFramePeer>(title_);
        nativeWindow_->setBounds(getX(), getY(), getWidth(), getHeight());
        configureNativeEvents();
        const auto peer =
            ::jxx::CAST<::jxx::gui::internal::WxFramePeer>(nativeWindow_);
        if (peer != nullptr)
            peer->installComponents(::jxx::CAST<Container>(thisPtr()));
        displayable_ = true;
    }

    void Dialog::validate()
    {
        Container::validate();
        if (nativeWindow_ == nullptr) return;
        const auto peer =
            ::jxx::CAST<::jxx::gui::internal::WxFramePeer>(nativeWindow_);
        if (peer != nullptr)
            peer->installComponents(::jxx::CAST<Container>(thisPtr()));
    }

    ::jxx::Ptr<::jxx::lang::String> Dialog::paramString() const
    {
        std::ostringstream out;
        out << "title=" << (title_ != nullptr ? title_->utf8() : "")
            << ",modal=" << (isModal() ? "true" : "false")
            << ",modalityType=" << modalityType_
            << ",resizable=" << (resizable_ ? "true" : "false")
            << ",undecorated=" << (undecorated_ ? "true" : "false");
        return ::jxx::NEW<::jxx::lang::String>(out.str());
    }
}
