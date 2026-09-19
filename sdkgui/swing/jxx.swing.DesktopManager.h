#pragma once

#include "lang/jxx.lang.ClassInfo.h"

namespace jxx::swing
{
    class JInternalFrame;

    class DesktopManager : public ::jxx::lang::InterfaceBase<DesktopManager>
    {
    public:
        ~DesktopManager() override = default;
        virtual void openFrame(const ::jxx::Ptr<JInternalFrame>& frame) = 0;
        virtual void closeFrame(const ::jxx::Ptr<JInternalFrame>& frame) = 0;
        virtual void maximizeFrame(const ::jxx::Ptr<JInternalFrame>& frame) = 0;
        virtual void minimizeFrame(const ::jxx::Ptr<JInternalFrame>& frame) = 0;
        virtual void iconifyFrame(const ::jxx::Ptr<JInternalFrame>& frame) = 0;
        virtual void deiconifyFrame(const ::jxx::Ptr<JInternalFrame>& frame) = 0;
        virtual void activateFrame(const ::jxx::Ptr<JInternalFrame>& frame) = 0;
        virtual void deactivateFrame(const ::jxx::Ptr<JInternalFrame>& frame) = 0;
        virtual void dragFrame(const ::jxx::Ptr<JInternalFrame>& frame,
            ::jxx::lang::jint x, ::jxx::lang::jint y) = 0;
        virtual void resizeFrame(const ::jxx::Ptr<JInternalFrame>& frame,
            ::jxx::lang::jint x, ::jxx::lang::jint y,
            ::jxx::lang::jint width, ::jxx::lang::jint height) = 0;
    };
}
