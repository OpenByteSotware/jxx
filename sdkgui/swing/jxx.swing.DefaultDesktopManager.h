#pragma once

#include "lang/jxx.lang.Object.h"
#include "swing/jxx.swing.DesktopManager.h"

namespace jxx::swing
{
    class DefaultDesktopManager : public ::jxx::lang::ClassBase<
        DefaultDesktopManager, ::jxx::lang::Object, DesktopManager>
    {
    public:
        using JxxSuper = ::jxx::lang::Object;
        using Super = ::jxx::lang::ClassBase<DefaultDesktopManager,
            JxxSuper, DesktopManager>;
        void openFrame(const ::jxx::Ptr<JInternalFrame>& frame) override;
        void closeFrame(const ::jxx::Ptr<JInternalFrame>& frame) override;
        void maximizeFrame(const ::jxx::Ptr<JInternalFrame>& frame) override;
        void minimizeFrame(const ::jxx::Ptr<JInternalFrame>& frame) override;
        void iconifyFrame(const ::jxx::Ptr<JInternalFrame>& frame) override;
        void deiconifyFrame(const ::jxx::Ptr<JInternalFrame>& frame) override;
        void activateFrame(const ::jxx::Ptr<JInternalFrame>& frame) override;
        void deactivateFrame(const ::jxx::Ptr<JInternalFrame>& frame) override;
        void dragFrame(const ::jxx::Ptr<JInternalFrame>& frame,
            ::jxx::lang::jint x, ::jxx::lang::jint y) override;
        void resizeFrame(const ::jxx::Ptr<JInternalFrame>& frame,
            ::jxx::lang::jint x, ::jxx::lang::jint y,
            ::jxx::lang::jint width, ::jxx::lang::jint height) override;
    };
}
