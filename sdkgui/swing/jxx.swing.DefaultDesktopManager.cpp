#include "swing/jxx.swing.DefaultDesktopManager.h"
#include "swing/jxx.swing.JInternalFrame.h"

namespace jxx::swing
{
    void DefaultDesktopManager::openFrame(
        const ::jxx::Ptr<JInternalFrame>& frame)
    { if (frame) frame->setVisible(true); }
    void DefaultDesktopManager::closeFrame(
        const ::jxx::Ptr<JInternalFrame>& frame)
    { if (frame) frame->dispose(); }
    void DefaultDesktopManager::maximizeFrame(
        const ::jxx::Ptr<JInternalFrame>& frame)
    { if (frame) frame->setMaximum(true); }
    void DefaultDesktopManager::minimizeFrame(
        const ::jxx::Ptr<JInternalFrame>& frame)
    { if (frame) frame->setMaximum(false); }
    void DefaultDesktopManager::iconifyFrame(
        const ::jxx::Ptr<JInternalFrame>& frame)
    { if (frame) frame->setIcon(true); }
    void DefaultDesktopManager::deiconifyFrame(
        const ::jxx::Ptr<JInternalFrame>& frame)
    { if (frame) frame->setIcon(false); }
    void DefaultDesktopManager::activateFrame(
        const ::jxx::Ptr<JInternalFrame>& frame)
    { if (frame) frame->setSelected(true); }
    void DefaultDesktopManager::deactivateFrame(
        const ::jxx::Ptr<JInternalFrame>& frame)
    { if (frame) frame->setSelected(false); }
    void DefaultDesktopManager::dragFrame(
        const ::jxx::Ptr<JInternalFrame>& frame,
        ::jxx::lang::jint x, ::jxx::lang::jint y)
    { if (frame) frame->setLocation(x, y); }
    void DefaultDesktopManager::resizeFrame(
        const ::jxx::Ptr<JInternalFrame>& frame,
        ::jxx::lang::jint x, ::jxx::lang::jint y,
        ::jxx::lang::jint width, ::jxx::lang::jint height)
    { if (frame) frame->setBounds(x, y, width, height); }
}
