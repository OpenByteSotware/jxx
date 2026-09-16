#pragma once

#include "awt/event/jxx.awt.event.ActionListener.h"
#include "awt/event/jxx.awt.event.AdjustmentListener.h"
#include "awt/event/jxx.awt.event.ComponentListener.h"
#include "awt/event/jxx.awt.event.ContainerListener.h"
#include "awt/event/jxx.awt.event.FocusListener.h"
#include "awt/event/jxx.awt.event.HierarchyBoundsListener.h"
#include "awt/event/jxx.awt.event.HierarchyListener.h"
#include "awt/event/jxx.awt.event.InputMethodListener.h"
#include "awt/event/jxx.awt.event.ItemListener.h"
#include "awt/event/jxx.awt.event.KeyListener.h"
#include "awt/event/jxx.awt.event.MouseListener.h"
#include "awt/event/jxx.awt.event.MouseMotionListener.h"
#include "awt/event/jxx.awt.event.MouseWheelListener.h"
#include "awt/event/jxx.awt.event.TextListener.h"
#include "awt/event/jxx.awt.event.WindowFocusListener.h"
#include "awt/event/jxx.awt.event.WindowListener.h"
#include "awt/event/jxx.awt.event.WindowStateListener.h"
#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Object.h"

namespace jxx::awt
{
    class AWTEventMulticaster : public ::jxx::lang::ClassBase<AWTEventMulticaster,
            ::jxx::lang::Object,
            ::jxx::awt::event::ActionListener,
            ::jxx::awt::event::AdjustmentListener,
            ::jxx::awt::event::ComponentListener,
            ::jxx::awt::event::ContainerListener,
            ::jxx::awt::event::FocusListener,
            ::jxx::awt::event::HierarchyBoundsListener,
            ::jxx::awt::event::HierarchyListener,
            ::jxx::awt::event::InputMethodListener,
            ::jxx::awt::event::ItemListener,
            ::jxx::awt::event::KeyListener,
            ::jxx::awt::event::MouseListener,
            ::jxx::awt::event::MouseMotionListener,
            ::jxx::awt::event::MouseWheelListener,
            ::jxx::awt::event::TextListener,
            ::jxx::awt::event::WindowFocusListener,
            ::jxx::awt::event::WindowListener,
            ::jxx::awt::event::WindowStateListener>
    {
    public:
        using JxxSuper = ::jxx::lang::Object;
        ~AWTEventMulticaster() override = default;
        static ::jxx::Ptr<::jxx::awt::event::ActionListener> add(const ::jxx::Ptr<::jxx::awt::event::ActionListener>& a, const ::jxx::Ptr<::jxx::awt::event::ActionListener>& b);
        static ::jxx::Ptr<::jxx::awt::event::ActionListener> remove(const ::jxx::Ptr<::jxx::awt::event::ActionListener>& listener, const ::jxx::Ptr<::jxx::awt::event::ActionListener>& oldListener);
        static ::jxx::Ptr<::jxx::awt::event::AdjustmentListener> add(const ::jxx::Ptr<::jxx::awt::event::AdjustmentListener>& a, const ::jxx::Ptr<::jxx::awt::event::AdjustmentListener>& b);
        static ::jxx::Ptr<::jxx::awt::event::AdjustmentListener> remove(const ::jxx::Ptr<::jxx::awt::event::AdjustmentListener>& listener, const ::jxx::Ptr<::jxx::awt::event::AdjustmentListener>& oldListener);
        static ::jxx::Ptr<::jxx::awt::event::ComponentListener> add(const ::jxx::Ptr<::jxx::awt::event::ComponentListener>& a, const ::jxx::Ptr<::jxx::awt::event::ComponentListener>& b);
        static ::jxx::Ptr<::jxx::awt::event::ComponentListener> remove(const ::jxx::Ptr<::jxx::awt::event::ComponentListener>& listener, const ::jxx::Ptr<::jxx::awt::event::ComponentListener>& oldListener);
        static ::jxx::Ptr<::jxx::awt::event::ContainerListener> add(const ::jxx::Ptr<::jxx::awt::event::ContainerListener>& a, const ::jxx::Ptr<::jxx::awt::event::ContainerListener>& b);
        static ::jxx::Ptr<::jxx::awt::event::ContainerListener> remove(const ::jxx::Ptr<::jxx::awt::event::ContainerListener>& listener, const ::jxx::Ptr<::jxx::awt::event::ContainerListener>& oldListener);
        static ::jxx::Ptr<::jxx::awt::event::FocusListener> add(const ::jxx::Ptr<::jxx::awt::event::FocusListener>& a, const ::jxx::Ptr<::jxx::awt::event::FocusListener>& b);
        static ::jxx::Ptr<::jxx::awt::event::FocusListener> remove(const ::jxx::Ptr<::jxx::awt::event::FocusListener>& listener, const ::jxx::Ptr<::jxx::awt::event::FocusListener>& oldListener);
        static ::jxx::Ptr<::jxx::awt::event::HierarchyBoundsListener> add(const ::jxx::Ptr<::jxx::awt::event::HierarchyBoundsListener>& a, const ::jxx::Ptr<::jxx::awt::event::HierarchyBoundsListener>& b);
        static ::jxx::Ptr<::jxx::awt::event::HierarchyBoundsListener> remove(const ::jxx::Ptr<::jxx::awt::event::HierarchyBoundsListener>& listener, const ::jxx::Ptr<::jxx::awt::event::HierarchyBoundsListener>& oldListener);
        static ::jxx::Ptr<::jxx::awt::event::HierarchyListener> add(const ::jxx::Ptr<::jxx::awt::event::HierarchyListener>& a, const ::jxx::Ptr<::jxx::awt::event::HierarchyListener>& b);
        static ::jxx::Ptr<::jxx::awt::event::HierarchyListener> remove(const ::jxx::Ptr<::jxx::awt::event::HierarchyListener>& listener, const ::jxx::Ptr<::jxx::awt::event::HierarchyListener>& oldListener);
        static ::jxx::Ptr<::jxx::awt::event::InputMethodListener> add(const ::jxx::Ptr<::jxx::awt::event::InputMethodListener>& a, const ::jxx::Ptr<::jxx::awt::event::InputMethodListener>& b);
        static ::jxx::Ptr<::jxx::awt::event::InputMethodListener> remove(const ::jxx::Ptr<::jxx::awt::event::InputMethodListener>& listener, const ::jxx::Ptr<::jxx::awt::event::InputMethodListener>& oldListener);
        static ::jxx::Ptr<::jxx::awt::event::ItemListener> add(const ::jxx::Ptr<::jxx::awt::event::ItemListener>& a, const ::jxx::Ptr<::jxx::awt::event::ItemListener>& b);
        static ::jxx::Ptr<::jxx::awt::event::ItemListener> remove(const ::jxx::Ptr<::jxx::awt::event::ItemListener>& listener, const ::jxx::Ptr<::jxx::awt::event::ItemListener>& oldListener);
        static ::jxx::Ptr<::jxx::awt::event::KeyListener> add(const ::jxx::Ptr<::jxx::awt::event::KeyListener>& a, const ::jxx::Ptr<::jxx::awt::event::KeyListener>& b);
        static ::jxx::Ptr<::jxx::awt::event::KeyListener> remove(const ::jxx::Ptr<::jxx::awt::event::KeyListener>& listener, const ::jxx::Ptr<::jxx::awt::event::KeyListener>& oldListener);
        static ::jxx::Ptr<::jxx::awt::event::MouseListener> add(const ::jxx::Ptr<::jxx::awt::event::MouseListener>& a, const ::jxx::Ptr<::jxx::awt::event::MouseListener>& b);
        static ::jxx::Ptr<::jxx::awt::event::MouseListener> remove(const ::jxx::Ptr<::jxx::awt::event::MouseListener>& listener, const ::jxx::Ptr<::jxx::awt::event::MouseListener>& oldListener);
        static ::jxx::Ptr<::jxx::awt::event::MouseMotionListener> add(const ::jxx::Ptr<::jxx::awt::event::MouseMotionListener>& a, const ::jxx::Ptr<::jxx::awt::event::MouseMotionListener>& b);
        static ::jxx::Ptr<::jxx::awt::event::MouseMotionListener> remove(const ::jxx::Ptr<::jxx::awt::event::MouseMotionListener>& listener, const ::jxx::Ptr<::jxx::awt::event::MouseMotionListener>& oldListener);
        static ::jxx::Ptr<::jxx::awt::event::MouseWheelListener> add(const ::jxx::Ptr<::jxx::awt::event::MouseWheelListener>& a, const ::jxx::Ptr<::jxx::awt::event::MouseWheelListener>& b);
        static ::jxx::Ptr<::jxx::awt::event::MouseWheelListener> remove(const ::jxx::Ptr<::jxx::awt::event::MouseWheelListener>& listener, const ::jxx::Ptr<::jxx::awt::event::MouseWheelListener>& oldListener);
        static ::jxx::Ptr<::jxx::awt::event::TextListener> add(const ::jxx::Ptr<::jxx::awt::event::TextListener>& a, const ::jxx::Ptr<::jxx::awt::event::TextListener>& b);
        static ::jxx::Ptr<::jxx::awt::event::TextListener> remove(const ::jxx::Ptr<::jxx::awt::event::TextListener>& listener, const ::jxx::Ptr<::jxx::awt::event::TextListener>& oldListener);
        static ::jxx::Ptr<::jxx::awt::event::WindowFocusListener> add(const ::jxx::Ptr<::jxx::awt::event::WindowFocusListener>& a, const ::jxx::Ptr<::jxx::awt::event::WindowFocusListener>& b);
        static ::jxx::Ptr<::jxx::awt::event::WindowFocusListener> remove(const ::jxx::Ptr<::jxx::awt::event::WindowFocusListener>& listener, const ::jxx::Ptr<::jxx::awt::event::WindowFocusListener>& oldListener);
        static ::jxx::Ptr<::jxx::awt::event::WindowListener> add(const ::jxx::Ptr<::jxx::awt::event::WindowListener>& a, const ::jxx::Ptr<::jxx::awt::event::WindowListener>& b);
        static ::jxx::Ptr<::jxx::awt::event::WindowListener> remove(const ::jxx::Ptr<::jxx::awt::event::WindowListener>& listener, const ::jxx::Ptr<::jxx::awt::event::WindowListener>& oldListener);
        static ::jxx::Ptr<::jxx::awt::event::WindowStateListener> add(const ::jxx::Ptr<::jxx::awt::event::WindowStateListener>& a, const ::jxx::Ptr<::jxx::awt::event::WindowStateListener>& b);
        static ::jxx::Ptr<::jxx::awt::event::WindowStateListener> remove(const ::jxx::Ptr<::jxx::awt::event::WindowStateListener>& listener, const ::jxx::Ptr<::jxx::awt::event::WindowStateListener>& oldListener);
        void actionPerformed(const ::jxx::Ptr<::jxx::awt::event::ActionEvent>& event) override;
        void adjustmentValueChanged(const ::jxx::Ptr<::jxx::awt::event::AdjustmentEvent>& event) override;
        void componentResized(const ::jxx::Ptr<::jxx::awt::event::ComponentEvent>& event) override;
        void componentMoved(const ::jxx::Ptr<::jxx::awt::event::ComponentEvent>& event) override;
        void componentShown(const ::jxx::Ptr<::jxx::awt::event::ComponentEvent>& event) override;
        void componentHidden(const ::jxx::Ptr<::jxx::awt::event::ComponentEvent>& event) override;
        void componentAdded(const ::jxx::Ptr<::jxx::awt::event::ContainerEvent>& event) override;
        void componentRemoved(const ::jxx::Ptr<::jxx::awt::event::ContainerEvent>& event) override;
        void focusGained(const ::jxx::Ptr<::jxx::awt::event::FocusEvent>& event) override;
        void focusLost(const ::jxx::Ptr<::jxx::awt::event::FocusEvent>& event) override;
        void ancestorMoved(const ::jxx::Ptr<::jxx::awt::event::HierarchyEvent>& event) override;
        void ancestorResized(const ::jxx::Ptr<::jxx::awt::event::HierarchyEvent>& event) override;
        void hierarchyChanged(const ::jxx::Ptr<::jxx::awt::event::HierarchyEvent>& event) override;
        void inputMethodTextChanged(const ::jxx::Ptr<::jxx::awt::event::InputMethodEvent>& event) override;
        void caretPositionChanged(const ::jxx::Ptr<::jxx::awt::event::InputMethodEvent>& event) override;
        void itemStateChanged(const ::jxx::Ptr<::jxx::awt::event::ItemEvent>& event) override;
        void keyTyped(const ::jxx::Ptr<::jxx::awt::event::KeyEvent>& event) override;
        void keyPressed(const ::jxx::Ptr<::jxx::awt::event::KeyEvent>& event) override;
        void keyReleased(const ::jxx::Ptr<::jxx::awt::event::KeyEvent>& event) override;
        void mouseClicked(const ::jxx::Ptr<::jxx::awt::event::MouseEvent>& event) override;
        void mousePressed(const ::jxx::Ptr<::jxx::awt::event::MouseEvent>& event) override;
        void mouseReleased(const ::jxx::Ptr<::jxx::awt::event::MouseEvent>& event) override;
        void mouseEntered(const ::jxx::Ptr<::jxx::awt::event::MouseEvent>& event) override;
        void mouseExited(const ::jxx::Ptr<::jxx::awt::event::MouseEvent>& event) override;
        void mouseDragged(const ::jxx::Ptr<::jxx::awt::event::MouseEvent>& event) override;
        void mouseMoved(const ::jxx::Ptr<::jxx::awt::event::MouseEvent>& event) override;
        void mouseWheelMoved(const ::jxx::Ptr<::jxx::awt::event::MouseWheelEvent>& event) override;
        void textValueChanged(const ::jxx::Ptr<::jxx::awt::event::TextEvent>& event) override;
        void windowGainedFocus(const ::jxx::Ptr<::jxx::awt::event::WindowEvent>& event) override;
        void windowLostFocus(const ::jxx::Ptr<::jxx::awt::event::WindowEvent>& event) override;
        void windowOpened(const ::jxx::Ptr<::jxx::awt::event::WindowEvent>& event) override;
        void windowClosing(const ::jxx::Ptr<::jxx::awt::event::WindowEvent>& event) override;
        void windowClosed(const ::jxx::Ptr<::jxx::awt::event::WindowEvent>& event) override;
        void windowIconified(const ::jxx::Ptr<::jxx::awt::event::WindowEvent>& event) override;
        void windowDeiconified(const ::jxx::Ptr<::jxx::awt::event::WindowEvent>& event) override;
        void windowActivated(const ::jxx::Ptr<::jxx::awt::event::WindowEvent>& event) override;
        void windowDeactivated(const ::jxx::Ptr<::jxx::awt::event::WindowEvent>& event) override;
        void windowStateChanged(const ::jxx::Ptr<::jxx::awt::event::WindowEvent>& event) override;
    protected:
        AWTEventMulticaster(const ::jxx::Ptr<::jxx::util::EventListener>& a, const ::jxx::Ptr<::jxx::util::EventListener>& b);
        ::jxx::Ptr<::jxx::util::EventListener> remove(const ::jxx::Ptr<::jxx::util::EventListener>& oldListener);
        static ::jxx::Ptr<::jxx::util::EventListener> addInternal(const ::jxx::Ptr<::jxx::util::EventListener>& a, const ::jxx::Ptr<::jxx::util::EventListener>& b);
        static ::jxx::Ptr<::jxx::util::EventListener> removeInternal(const ::jxx::Ptr<::jxx::util::EventListener>& listener, const ::jxx::Ptr<::jxx::util::EventListener>& oldListener);
        ::jxx::Ptr<::jxx::util::EventListener> a;
        ::jxx::Ptr<::jxx::util::EventListener> b;
    };
}
